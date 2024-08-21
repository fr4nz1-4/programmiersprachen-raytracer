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

Color background_color = {1.0f, 1.0f, 1.0f};
//Color background_color = {0.0f, 0.0f, 0.0f};
Material red = {"red", Color{1, 0, 0}, Color{1, 0, 0}, Color{1, 0, 0}, 20.0f};

void Renderer::render() {

    Camera camera;
    glm::vec3 light_position = {300.0f, 300.0f, -110.0f}; // Beispiel für Lichtposition
//    glm::vec3 light_position = camera.origin;
    Color light_intensity = {0.5f, 0.5f, 0.5f};
    Color light_intensity_ambient = {0.3f,0.3f,0.3f};// Lichtintensität

    for (unsigned y = 0; y < height_; ++y) {
        for (unsigned x = 0; x < width_; ++x) {
            Pixel p = {x, y};

            // Ray from camera through pixel
            glm::vec3 pixel_position = {x, y, 0.0f}; // Position auf Bildfläche
            glm::vec3 ray_direction = glm::normalize(pixel_position - camera.origin); // Strahlrichtung

            Ray ray = {camera.origin, ray_direction}; // Erstelle den Strahl

            p.color = background_color; // Initialisiere mit Hintergrundfarbe
            float closest_distance = std::numeric_limits<float>::max(); // Um den nächsten Schnittpunkt zu finden

            // Überprüfe alle Objekte in der Szene
            for (auto const& shape : scene_.shape_container) {
                auto hit = shape->intersect(ray);
                if (hit.intersection) {

                    // Spiegelnde/Spekulare Reflexion
                    glm::vec3 v = glm::normalize(camera.origin - hit.intersection_point); // Blickvektor
                    glm::vec3 I = glm::normalize(light_position - hit.intersection_point); // Lichtstrahlvektor
                    glm::vec3 normal = shape->normal(hit.intersection_point); // Normalenvektor
                    glm::vec3 r = glm::normalize(2.0f * glm::dot(normal, I) * normal - I); // Reflexionsvektor

                    float distance = glm::length(hit.intersection_point - camera.origin);

                    // Falls diese Intersektion näher ist als die vorherige, aktualisiere die Farbe
                    if (distance < closest_distance) {
                        closest_distance = distance;

                        // Ambienter Beleuchtungsanteil (Ia * ka)
                        Color ambient_component = {light_intensity_ambient.r * shape->get_Material()->ka.r, light_intensity_ambient.g * shape->get_Material()->ka.g, light_intensity_ambient.b * shape->get_Material()->ka.b};

                        // Diffuse Reflexion
                        glm::vec3 l = glm::normalize(light_position - hit.intersection_point); // Lichtvektor
                        float diffuse_factor = std::max(glm::dot(normal, l), 0.0f);
                        Color diffuse_i = {light_intensity.r * shape->get_Material()->kd.r, light_intensity.g * shape->get_Material()->kd.g, light_intensity.b * shape->get_Material()->kd.b};
                        Color diffuse_component = {diffuse_i.r * diffuse_factor, diffuse_i.g * diffuse_factor, diffuse_i.b * diffuse_factor};

                        // Spekulare Reflexion
                        float spec_exp = shape->get_Material()->m; // Spekularexponent
                        float specular_factor = pow(std::max(glm::dot(r, v), 0.0f), spec_exp);
                        Color specular_i = {light_intensity.r * shape->get_Material()->ks.r, light_intensity.g * shape->get_Material()->ks.g, light_intensity.b * shape->get_Material()->ks.b};
                        Color specular_component = {shape->get_Material()->ks.r * specular_factor, shape->get_Material()->ks.g * specular_factor, shape->get_Material()->ks.b * specular_factor};

                        // Setze die endgültige Farbe des Pixels
                        p.color = ambient_component + diffuse_component + specular_component;
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