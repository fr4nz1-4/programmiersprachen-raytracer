// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#include "renderer.hpp"

Renderer::Renderer(unsigned w, unsigned h, std::string const& file)
  : width_(w)
  , height_(h)
  , color_buffer_(w*h, Color{0.0, 0.0, 0.0})
  , filename_(file)
  , ppm_(width_, height_)
{}

Color background_color = {1.0f, 1.0f, 1.0f};
Material red = {"red", Color{1, 0, 0}, Color{1, 0, 0}, Color{1, 0, 0}, 20.0f};
std::shared_ptr<Material> red_ptr = std::make_shared<Material>(red);

void Renderer::render(){
    Sphere sphere1 = {"wow", red_ptr, {20.0f, 20.0f, 20.0f}, 20.0f};  // Eine Kugel weit in die Szene setzen
    glm::vec3 camera_origin = {0.0f, 0.0f, -100.0f};  // Setze die Kamera weit vor die Szene

    for (unsigned y = 0; y < height_; ++y) {
        for (unsigned x = 0; x < width_; ++x) {
            Pixel p = {x, y};

            // Ray from camera through the pixel
            glm::vec3 pixel_position = {(float)x, (float)y, 0.0f}; // Position auf der Bildfläche
            glm::vec3 ray_direction = glm::normalize(pixel_position - camera_origin); // Richtung vom Ursprung (Kamera) zum Pixel

            Ray ray = {camera_origin, ray_direction};  // Erstelle den Strahl

            if (sphere1.intersect(ray).intersection) {
                p.color = {1.0f, 0.0f, 0.0f};  // Rot färben, wenn Kugel getroffen wird
            } else {
                p.color = {background_color};  // Andernfalls Hintergrundfarbe
            }

            write(p);  // Pixel in das Bild schreiben
        }
    }
    ppm_.save(filename_);
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
