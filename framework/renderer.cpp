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

Renderer::Renderer(unsigned w, unsigned h, std::string const& file, Scene const& scene)
  : width_(w)
  , height_(h)
  , color_buffer_(w*h, Color{0.0, 0.0, 0.0})
  , filename_(file)
  , ppm_(width_, height_)
  , scene_(scene)
{}

Color background_color = {1.0f, 1.0f, 1.0f};
//Color background_color = {0.0f, 0.0f, 0.0f};
Material red = {"red", Color{1, 0, 0}, Color{1, 0, 0}, Color{1, 0, 0}, 20.0f};

void Renderer::render() {

//    Light light = {{0.0f, -300.0f, -150.0f}, {1.0f, 1.0f, 1.0f}};
////    Light light2 = {{00.0f, -400.0f, -150.0f}, {0.5f, 0.5f, 0.5f}};
//    Light light2 = {{camera.origin}, {0.5f, 0.5f, 0.5f}};
//
//    std::vector<Light> light_container;
//    light_container.push_back(light);
//    light_container.push_back(light2);

//    glm::vec3 light_position = camera.origin;
    Color ambient_factor = {0.5f, 0.5f, 0.5f}; // Lichtintensität

    for (unsigned y = 0; y < height_; ++y) {
        for (unsigned x = 0; x < width_; ++x) {
            Pixel p = {x, y};
            float xray = (2.0f * p.x) / width_ - 1.0f;
            float yray = 1.0f - (2.0f * p.y) / height_;
            float zray = -1.0f;

            // Ray from camera through pixel
            glm::vec3 pixel_position = {x, y, 0.0f}; // Position auf Bildfläche
            glm::vec3 ray_direction = glm::normalize(glm::vec3{xray, yray, zray} - scene_.camera_container[0]->origin);
            //glm::normalize(pixel_position - camera.origin); // Strahlrichtung

            Ray ray = {scene_.camera_container[0]->origin, ray_direction}; // Erstelle den Strahl

            p.color = background_color; // Initialisiere mit Hintergrundfarbe
            float closest_distance = std::numeric_limits<float>::max(); // Um den nächsten Schnittpunkt zu finden

            // Überprüfe alle Objekte in der Szene
            for (auto const& shape : scene_.shape_container) {
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

void Renderer::write(Pixel const& p)
{
    // flip pixels, because of opengl glDrawPixels
    size_t buf_pos = (width_*p.y + p.x);
    if (buf_pos >= color_buffer_.size() || (int)buf_pos < 0) {
        std::cerr << "Fatal Error Renderer::write(Pixel p) : "
        << "pixel out of ppm_ : "
        << (int)p.x << "," << (int)p.y
        << std::endl;
    } else {
        color_buffer_[buf_pos] = p.color;
    }
    ppm_.write(p);
}