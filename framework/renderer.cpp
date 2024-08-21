// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#include "renderer.hpp"

Renderer::Renderer(unsigned w, unsigned h, std::string const& file, Scene const& scene)
  : width_(w)
  , height_(h)
  , color_buffer_(w*h, Color{0.0, 0.0, 0.0})
  , filename_(file)
  , ppm_(width_, height_)
  , scene_(scene)
{}

//Color background_color = {1.0f, 1.0f, 1.0f};
Color background_color = {0.0f, 0.0f, 0.0f};
Material red = {"red", Color{1, 0, 0}, Color{1, 0, 0}, Color{1, 0, 0}, 20.0f};

void Renderer::render() {

    Camera camera;
//    glm::vec3 camera_origin = {width_/2, height_/2, -200.0f};  // Setze die Kamera weit vor die Szene
//    glm::vec3 camera_origin = {0.0f, 0.0f, 0.0f};  // Setze die Kamera weit vor die Szene
//    glm::vec3 light_position = {300.0f, 200.0f , 200};
    glm::vec3 light_position = camera.origin;
    Color light_intensity = {0.5f, 0.5f, 0.5f}; // Beispielintensität für das Licht

    for (unsigned y = 0; y < height_; ++y) {
        for (unsigned x = 0; x < width_; ++x) {
            Pixel p = {x, y};

            // Ray from camera through pixel
            glm::vec3 pixel_position = {x, y, 0.0f}; // Position auf Bildfläche
            glm::vec3 ray_direction = glm::normalize(pixel_position - camera.origin); // stellt Vektor aus zwei Punkten auf

            Ray ray = {camera.origin, ray_direction}; // Erstellt den Strahl

            p.color = background_color; // Initialisiere mit Hintergrundfarbe
            float closest_distance = std::numeric_limits<float>::max(); // Um den nächsten Schnittpunkt zu finden

            // Überprüfe alle Objekte in der Szene
            for (auto const& shape : scene_.shape_container) {
                auto hit = shape->intersect(ray);
                if (hit.intersection) {
                    // Berechne distanz zur Kamera
                    float distance = glm::length(hit.intersection_point - camera.origin);

                    // Falls diese Intersektion näher ist als die vorherige, aktualisiere die Farbe
                    if (distance < closest_distance) {
                        closest_distance = distance;

                        // Ambienter Beleuchtungsanteil (Ia * ka)
                        Color ambient_component = {light_intensity.r * shape->get_Material()->ka.r, light_intensity.g * shape->get_Material()->ka.g, light_intensity.b * shape->get_Material()->ka.b};
                        Color diffuse_component = {};
                        glm::vec3 l = glm::normalize(light_position - hit.intersection_point); // Lichtvektor
//
                        glm::vec3 normal = shape->normal(hit.intersection_point);
                        float diffuse_factor = std::max(glm::dot(normal, l), 0.0f);
                        // Ip * kd
                        Color diffuse_i = {light_intensity.r * shape->get_Material()->kd.r, light_intensity.g * shape->get_Material()->kd.g, light_intensity.b * shape->get_Material()->kd.b};
                        diffuse_component = {diffuse_i.r * diffuse_factor, diffuse_i.g * diffuse_factor, diffuse_i.b * diffuse_factor};

                        p.color = ambient_component + diffuse_component;
                    }
                }
            }
            write(p);  // Schreibe die Farbe des Pixels in das Bild
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