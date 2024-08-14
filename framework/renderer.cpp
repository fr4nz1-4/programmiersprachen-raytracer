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
#include <algorithm>

Renderer::Renderer(unsigned w, unsigned h, std::string const& file, Scene const& scene)
  : width_(w)
  , height_(h)
  , color_buffer_(w*h, Color{0.0, 0.0, 0.0})
  , filename_(file)
  , ppm_(width_, height_)
  , scene_(scene)
{}

Color background_color = {1.0f, 1.0f, 1.0f};
Material red = {"red", Color{1, 0, 0}, Color{1, 0, 0}, Color{1, 0, 0}, 20.0f};
std::shared_ptr<Material> red_ptr = std::make_shared<Material>(red);


//void trace();

void Renderer::render() {
    //ambiente bumse
    //i_a = intensität ambiente licht i=intensität der beleuchtung
    //k_a = ambienter reflexionskoefizient
    //
    glm::vec3 camera_origin = {width_/2, height_/2, -100.0f};  // Setze die Kamera weit vor die Szene

    for (unsigned y = 0; y < height_; ++y) {
        for (unsigned x = 0; x < width_; ++x) {
            Pixel p = {x, y};

            // Ray from camera through the pixel
            glm::vec3 pixel_position = {x, y, 0.0f}; // Position auf Bildfläche
            glm::vec3 ray_direction = glm::normalize(
                    pixel_position - camera_origin); // stellt Vektor aus zwei Punkten auf

            Ray ray = {camera_origin, ray_direction}; // Erstellt den Strahl

            // Initialisiere mit Hintergrundfarbe
            p.color = background_color;
            float closest_distance = std::numeric_limits<float>::max(); // Um die nächste Intersektion zu finden

            // Überprüfe alle Objekte in der Szene
            for (auto const& shape : scene_.shape_container) {
                auto hit = shape->intersect(ray);
                if (hit.intersection) {
                    // Berechne die Distanz zur Kamera
                    float distance = glm::length(hit.intersection_point - camera_origin);

                    // Falls diese Intersektion näher ist als die vorherige, aktualisiere die Farbe
                    if (distance < closest_distance) {
                        closest_distance = distance;
                        p.color = shape->get_Material()->kd * ; // Verwende die Farbe des Materials
                    }
                }
            }

            write(p);  // Schreibe die Farbe des Pixels in das Bild
        }
    }

//    ppm_.save(filename_); // Speichere das Bild nach dem Rendern aller Pixel
}
    // TODO: zu langsam, muss optimiert werden
//    for (unsigned y = 0; y < height_; ++y) {
//        for (unsigned x = 0; x < width_; ++x) {
//            Pixel p = {x, y};
//            auto it = std::find_if(pixel_vector.begin(), pixel_vector.end(),
//                                   [&p](const Pixel& other){return p.x == other.x && p.y == other.y;});
//            if (it != pixel_vector.end()) {
//                p.color = it->color;
//                write(p);
//            } else {
//                p.color = {background_color};
//                write(p);
//            }
//        }
//    }
//}

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
