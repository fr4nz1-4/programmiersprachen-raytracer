// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#include "renderer.hpp"
#include <vector>


#define PI 3.14159265358979323846f

Renderer::Renderer(unsigned w, unsigned h, std::string const& file, Scene const& scene)
        : width_(w)
        , height_(h)
        , color_buffer_(w * h, Color{0.0, 0.0, 0.0})
        , filename_(file), ppm_(width_, height_)
        , scene_(scene) {}

Color background_color = {1.0f, 1.0f, 1.0f};
//Color background_color = {0.0f, 0.0f, 0.0f};

Ray Renderer::transform_ray(glm::mat4 const& mat, Ray const& ray) {

    glm::vec4 transformed_origin = mat * glm::vec4{ray.origin, 1.0f};
    glm::vec4 transformed_direction = mat * glm::vec4{ray.direction, 0.0f};

    return Ray{glm::vec3{transformed_origin}, (glm::vec3{transformed_direction})};
}


Color Renderer::shade(Ray const& ray, std::shared_ptr<Shape> const& shape, float const& distance) {
    glm::mat4 test_mat;
    HitPoint hit = shape->intersect(ray);
    
    
    glm::mat4 mattti = glm::transpose(shape->get_world_transformation_inv()) ;
    glm::vec4 result = shape->get_world_transformation() * glm::vec4{ hit.intersection_point, 1.0f }; //0.0f oder 1.0f im vec 4 ?? zurücktransformierter hitpoint(intersection point)
    hit.intersection_point = glm::vec3{result}; //hit intersetion point wird auf den zurücktransformierten gesetzt 
    hit = transform(mattti,hit); //normale wid zurückberechnet 
    
    
    
//    glm::vec3 intersection_point = ray.origin + ray.direction * distance;
    

    Color ambient_factor = {0.5f, 0.5f, 0.5f};
    Color ambient_component = {ambient_factor.r * shape->get_Material()->ka.r,
                               ambient_factor.g * shape->get_Material()->ka.g,
                               ambient_factor.b * shape->get_Material()->ka.b};

    Color diffuse_component = {0, 0, 0};
    Color specular_component = {0, 0, 0};

    for (auto const& light : scene_.light_container) {
        glm::vec3 light_dir = glm::normalize(light->position - hit.intersection_point);
        glm::vec3 test2 = hit.intersection_point + 0.1f * hit.normale;

        Ray shadow_ray = {test2, light_dir};
        bool in_shadow = false;

        for (auto const& shape_other : scene_.shape_container) {
            if (shape != shape_other) {
                auto shadow_hit = shape_other->intersect(shadow_ray);
                if (shadow_hit.intersection) {
                    in_shadow = true;
                    break;
                }
            }
        }

        if (!in_shadow) {
            // Diffuse Beleuchtung
            float diffuse_factor = std::max(glm::dot(hit.normale, light_dir), 0.0f);
            Color diffuse_component_tmp = {
                    light->color.r * light->brightness * shape->get_Material()->kd.r * diffuse_factor,
                    light->color.g * light->brightness * shape->get_Material()->kd.g * diffuse_factor,
                    light->color.b * light->brightness * shape->get_Material()->kd.b * diffuse_factor};
            diffuse_component = diffuse_component + diffuse_component_tmp;

            // Spekulare Beleuchtung
            glm::vec3 r = glm::normalize(2.0f * glm::dot(hit.normale, light_dir) * hit.normale - light_dir);
            glm::vec3 v = glm::normalize(scene_.camera_container[0]->origin - hit.intersection_point);

            float specular_factor = std::pow(std::max(glm::dot(r, v), 0.0f), shape->get_Material()->m);
            Color specular_component_tmp = {
                    light->color.r * light->brightness * specular_factor,
                    light->color.g * light->brightness * specular_factor,
                    light->color.b * light->brightness * specular_factor};

            specular_component = specular_component + specular_component_tmp;
        }
    }
    return {ambient_component + diffuse_component + specular_component};
}

Color Renderer::trace(Ray const& ray) {
    float closest_distance = std::numeric_limits<float>::max();
    std::shared_ptr<Shape> closest_shape = nullptr;
    Ray raysin = ray;
    for (auto const& shape : scene_.shape_container) {
       
       
             raysin = transform_ray(shape->get_world_transformation_inv(), ray); //ray wird transformiert 
             HitPoint hit = shape->intersect(raysin); //hit mit transformed ray
           // glm::vec4 result = shape->get_world_transformation() * glm::vec4{hit.intersection_point, 1.0f} ; //0.0f oder 1.0f im vec 4 ?? zurücktransformierter hitpoint(intersection point)
           // hit.intersection_point = glm::vec3{result}; //hit intersetion point wird auf den zurücktransformierten gesetzt 
            

        if (hit.intersection) {
            float distance = glm::length(hit.intersection_point - scene_.camera_container[0]->origin);
            if (distance < closest_distance) {
                closest_distance = distance;
                closest_shape = shape;
            }
        }
    }

    if (closest_shape) {
        
        return shade(raysin, closest_shape, closest_distance);
    } else {
        return background_color;
    }
}

void Renderer::render() {
    Camera camera = *scene_.camera_container[0];

    for (unsigned y = 0; y < height_; ++y) {
        for (unsigned x = 0; x < width_; ++x) {
            Pixel p = {x, y};

            float d = (width_ / 2.0f) / std::tan((scene_.camera_container[0]->fov / 2) / 180 * PI);

            glm::vec3 u = glm::normalize(glm::cross(camera.direction, camera.up));
            glm::mat4 camera_mat;
            camera_mat[0] = glm::vec4{u, 0};
            camera_mat[1] = glm::vec4{glm::cross(u, (camera.direction)), 0};
            camera_mat[2] = glm::vec4{-(camera.direction), 0};
            camera_mat[3] = glm::vec4{camera.origin, 1};

            // Berechnung der Strahlenrichtung
            glm::vec4 ray_direction{glm::normalize(glm::vec3{(-(width_ / 2.0f) + float(p.x * 1.0f)),
                                                             (-(height_ / 2.0f) + float(p.y * 1.0f)),
                                                             - d}), 0};

            ray_direction = glm::normalize(camera_mat * ray_direction);

            p.color = background_color; // Initialisiere mit Hintergrundfarbe

            Ray ray = {camera.origin, glm::vec3{ray_direction}}; // Multiplikation mit Inverser Worldmatrix

            Color clr = trace(ray);

            // Hintergrundfarbe wird nicht verändert (oder in shade() anpassen)
            if (clr.r != background_color.r && clr.g != background_color.g && clr.b != background_color.b) {
                p.color = Color{clr.r / (clr.r + 1), clr.g / (clr.g + 1), clr.b / (clr.b + 1)};
            }

            // Hintergrundfarbe wird auch verändert -> weiß wird zu grau
            // p.color = Color{clr.r / (clr.r+1), clr.g / (clr.g+1), clr.b / (clr.b+1)};

            write(p);  // Schreibe die Farbe des Pixels in das Bild
        }
    }
//    ppm_.save(filename_); // Speichere das Bild nach dem Rendern aller Pixel
}

/* // alte render()-Methode
void Renderer::render() {
    Color ambient_factor = {0.5f, 0.5f, 0.5f}; // ambiente Lichtintensität
    Camera camera = *scene_.camera_container[0];

    for (unsigned y = 0; y < height_; ++y) {
        for (unsigned x = 0; x < width_; ++x) {
            Pixel p = {x, y};

            float d = (width_ / 2.0f) / std::tan((scene_.camera_container[0]->fov / 2) / 180 * PI);

            glm::vec3 u = glm::normalize(glm::cross(camera.direction, camera.up));
            glm::mat4 camera_mat;
            camera_mat[0] = glm::vec4{u, 0};
            camera_mat[1] = glm::vec4{glm::cross(u, (camera.direction)), 0};
            camera_mat[2] = glm::vec4{-(camera.direction), 0};
            camera_mat[3] = glm::vec4{camera.origin, 1};

            glm::vec4 ray_direction{glm::normalize(glm::vec3{(-(width_ / 2.0f) + float(p.x * 1.0f)), (-(height_ / 2.0f) + float(p.y * 1.0f)), - d}), 0};// Berechnung der Strahlenrichtung

            ray_direction = glm::normalize(camera_mat * ray_direction);
            glm::vec3 ray_direktion1 = glm::vec3{ray_direction};

            Ray ray = {scene_.camera_container[0]->origin, ray_direktion1};

            p.color = background_color; // Initialisiere mit Hintergrundfarbe
            float closest_distance = std::numeric_limits<float>::max(); // Um den nächsten Schnittpunkt zu finden

            // Überprüfe alle Objekte in der Szene
            for (auto const& shape: scene_.shape_container) {
                auto hit = shape->intersect(ray);
                if (hit.intersection) {
                    float distance = glm::length(hit.intersection_point - scene_.camera_container[0]->origin);
                    glm::vec3 normal = shape->normal(hit.intersection_point); // Normalenvektor

                    // Falls diese Intersektion näher ist als die vorherige, aktualisiere die Farbe
                    if (distance < closest_distance) {
                        closest_distance = distance;

                        // Ambiente Beleuchtung (Ia * ka)
                        Color ambient_component = {ambient_factor.r * shape->get_Material()->ka.r,
                                                   ambient_factor.g * shape->get_Material()->ka.g,
                                                   ambient_factor.b * shape->get_Material()->ka.b};

                        Color diffuse_component = {0,0,0};
                        Color specular_component = {0,0,0};
                        bool in_shadow = false; // Lichtquelle ist von kamera aus sichtbar (nicht im schatten)

                        for (auto const& tmp_light: scene_.light_container) {
                            glm::vec3 light_dir = glm::normalize(tmp_light->position - hit.intersection_point);
                            // TODO: test2 umbenennen
                            glm::vec3 test2 = {hit.intersection_point.x + shape->normal(hit.intersection_point).x,
                                               hit.intersection_point.y + shape->normal(hit.intersection_point).y,
                                               hit.intersection_point.z + shape->normal(hit.intersection_point).z};
                            Ray shadow_ray = {test2, light_dir};

                            for (auto const& shape_other: scene_.shape_container) {
                                if (shape != shape_other) { // Teste nicht gegen sich selbst
                                    auto shadow_hit = shape_other->intersect(shadow_ray);
                                    if (shadow_hit.intersection) {
                                        in_shadow = true;
                                        break;
                                    }
                                }
                            }

                            if (!in_shadow) {
                                // Diffuse Beleuchtung
                                float diffuse_factor = std::max(glm::dot(normal, light_dir), 0.0f);
                                Color diffuse_component_tmp = {
                                        tmp_light->color.r * tmp_light->brightness * shape->get_Material()->kd.r * diffuse_factor,
                                        tmp_light->color.g * tmp_light->brightness * shape->get_Material()->kd.g * diffuse_factor,
                                        tmp_light->color.b * tmp_light->brightness * shape->get_Material()->kd.b * diffuse_factor};
                                diffuse_component = diffuse_component + diffuse_component_tmp;

                                // Spekulare Beleuchtung
                                glm::vec3 r = glm::normalize(2.0f * glm::dot(normal, light_dir) * normal - light_dir);
                                glm::vec3 v = glm::normalize(scene_.camera_container[0]->origin - hit.intersection_point);

                                float specular_factor = std::pow(std::max(glm::dot(r, v), 0.0f), shape->get_Material()->m);

                                Color specular_component_tmp = {                                      // wenn einkommentiert hat reflektion die farbe der shape
                                        tmp_light->color.r * tmp_light->brightness * specular_factor, //* shape->get_Material()->ks.r
                                        tmp_light->color.g * tmp_light->brightness * specular_factor, //* shape->get_Material()->ks.g
                                        tmp_light->color.b * tmp_light->brightness * specular_factor //* shape->get_Material()->ks.b
                                        };
                                specular_component = (specular_component + specular_component_tmp);
                                std::cout<< shape->get_name() <<std::endl; // test fürs debugging
                            }
                        }
                        // Setze die endgültige Farbe des Pixels
                        Color cldr = ambient_component + diffuse_component + specular_component;
                        p.color = Color{cldr.r / (cldr.r+1), cldr.g / (cldr.g+1), cldr.b / (cldr.b+1)};

                    }
                }
            }
            write(p);  // Schreibe die Farbe des Pixels in das Bild  // Schreibe die Farbe des Pixels in das Bild
        }
    }
//    ppm_.save(filename_); // Speichere das Bild nach dem Rendern aller Pixel
}
*/

void Renderer::write(Pixel const &p) {
    // flip pixels, because of opengl glDrawPixels
    size_t buf_pos = (width_ * p.y + p.x);
    if (buf_pos >= color_buffer_.size() || (int) buf_pos < 0) {
        std::cerr << "Fatal Error Renderer::write(Pixel p) : "
                  << "pixel out of ppm_ : "
                  << (int) p.x << "," << (int) p.y
                  << std::endl;
    } else {
        color_buffer_[buf_pos] = p.color;
    }
    ppm_.write(p);
}
