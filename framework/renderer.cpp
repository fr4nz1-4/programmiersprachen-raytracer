// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#include "renderer.hpp"
#include <glm/gtx/string_cast.hpp>
#include <algorithm>
#include <vector>
#include <glm/gtx/string_cast.hpp>

#define PI 3.14159265358979323846f

Renderer::Renderer(unsigned w, unsigned h, std::string const& file, Scene const& scene)
        : width_(w)
        , height_(h)
        , color_buffer_(w * h, Color{0.0, 0.0, 0.0})
        , filename_(file), ppm_(width_, height_)
        , scene_(scene) {}

const Color background_color = {0.0f, 0.0f, 0.0f};
const int max_depth = 5; // Maximale Rekursionstiefe für Reflexionen

// transformiert ray vom
Ray Renderer::transform_ray(glm::mat4 const& mat, Ray const& ray) {

    glm::vec4 transformed_origin = mat * glm::vec4{ray.origin, 1.0f};
    glm::vec4 transformed_direction = mat * glm::vec4{ray.direction, 0.0f};

    return Ray{glm::vec3{transformed_origin}, (glm::vec3{transformed_direction})};
}

Color Renderer::shade(HitPoint const& closest_object_hitpoint, std::shared_ptr<Shape> const& shape, float distance, Ray const& ray, int depth) {
    HitPoint untransformed_hitpoint = closest_object_hitpoint;
    glm::mat4 world_mat = shape->get_world_transformation();
    glm::mat4 world_mat_inv = shape->get_world_transformation_inv();

    glm::mat4 transposed_inverse_world_mat = glm::transpose(shape->get_world_transformation_inv()) ;
    HitPoint transformed_hit_point = transform(world_mat, transposed_inverse_world_mat, untransformed_hitpoint); //normale wid zurücktransfomiert

    Color ambient_factor = {0.5f, 0.5f, 0.5f};
    Color diffuse_component = {0, 0, 0};
    Color specular_component = {0, 0, 0};

    // ambiente beleuchtung
    Color ambient_component = {ambient_factor.r * shape->get_Material()->ka.r,
                               ambient_factor.g * shape->get_Material()->ka.g,
                               ambient_factor.b * shape->get_Material()->ka.b};

    // Schattenberechnung und Beleuchtung für jede Lichtquelle
    for (auto const& light : scene_.light_container) {
        glm::vec3 light_dir = glm::normalize(light->position - transformed_hit_point.intersection_point);
        glm::vec3 test2 = transformed_hit_point.intersection_point + 0.1f * transformed_hit_point.normale;

        Ray shadow_ray = {test2, light_dir};
        bool in_shadow = false;

        // überprüft ob objekt im schatten liegt
        for (auto const& shape_other : scene_.shape_container) {
            if (shape != shape_other) {
                Ray transformed_shadow_ray = transform_ray(shape_other->get_world_transformation_inv(), shadow_ray);
                auto shadow_hit = shape_other->intersect(transformed_shadow_ray);
                if (shadow_hit.intersection) {
                    in_shadow = true;
                    break;
                }
            }
        }

        if (!in_shadow) {
            // Diffuse Beleuchtung
            float diffuse_factor = std::max(glm::dot(transformed_hit_point.normale, light_dir), 0.0f);
            Color diffuse_component_tmp = {
                    light->color.r * light->brightness * shape->get_Material()->kd.r * diffuse_factor,
                    light->color.g * light->brightness * shape->get_Material()->kd.g * diffuse_factor,
                    light->color.b * light->brightness * shape->get_Material()->kd.b * diffuse_factor};
            diffuse_component = diffuse_component + diffuse_component_tmp;

            // Spekulare Beleuchtung
            glm::vec3 r = glm::normalize(2.0f * glm::dot(transformed_hit_point.normale, light_dir) * transformed_hit_point.normale - light_dir);
            glm::vec3 v = glm::normalize(scene_.camera_container[0]->origin - transformed_hit_point.intersection_point);

            float specular_factor = std::pow(std::max(glm::dot(r, v), 0.0f), shape->get_Material()->m);
            Color specular_component_tmp = {
                    light->color.r * light->brightness * specular_factor,
                    light->color.g * light->brightness * specular_factor,
                    light->color.b * light->brightness * specular_factor};

            specular_component = specular_component + specular_component_tmp;
        }
    }

    Color local_color = ambient_component + diffuse_component + specular_component;

    // Reflexion
    float reflectivity = shape->get_Material()->reflectivity;
    if (reflectivity > 0.0f && depth < max_depth) {
        // Reflexionsrichtung berechnen
        glm::vec3 reflection_direction = glm::reflect(ray.direction, transformed_hit_point.normale);
        Ray reflected_ray{transformed_hit_point.intersection_point + 0.001f * transformed_hit_point.normale, reflection_direction};

        // Verfolge reflektierten Strahl
        Color reflected_color = trace(reflected_ray, depth + 1);

        // Reflexionsfarbe hinzufügen
        local_color.r = local_color.r + reflectivity * reflected_color.r;
        local_color.g = local_color.g + reflectivity * reflected_color.g;
        local_color.b = local_color.b + reflectivity * reflected_color.b;
    }

#if 1
        return local_color;
    #else
        glm::vec3 normal_color = untransformed_hitpoint.normale * glm::vec3(0.5f) + glm::vec3(0.5f);
        //std::cout << glm::to_string(normal_color) << std::endl;
        return { normal_color.x, normal_color.y, normal_color.z };
    #endif
}

Color Renderer::trace(Ray const& ray, int depth) {
    if (depth > max_depth) {
        return background_color;
    }

    HitPoint closest_hit;
    float closest_distance = std::numeric_limits<float>::max();
    std::shared_ptr<Shape> closest_shape = nullptr;

    // shape (und hitpoint) finden, die am nächsten an kamera dran ist
    for (auto const& shape : scene_.shape_container) {
        Ray transformed_ray = transform_ray(shape->get_world_transformation_inv(), ray);
        HitPoint hit = shape->intersect(transformed_ray);
        if (hit.intersection && hit.distance < closest_distance) {
            closest_hit = hit;
            closest_distance = hit.distance;
            closest_shape = shape;
        }
    }

    // wenn kein objekt getroffen
    if (!closest_hit.intersection) {
        return background_color;
    }
    // Farbe berechnen (Reflexionen werden nun in shade behandelt)
    return shade(closest_hit, closest_shape, closest_distance, ray, depth);
}

void Renderer::render() {
    Camera camera = *scene_.camera_container[0];

    for (unsigned y = 0; y < height_; ++y) {
        for (unsigned x = 0; x < width_; ++x) {
            Pixel p = {x, y};
            Color final_color = {0.0f, 0.0f, 0.0f};

            // abstand der kamera zur bilebene
            float d = (width_ / 2.0f) / std::tan((scene_.camera_container[0]->fov / 2.0f) / 180.0f * PI);

            glm::vec3 u = glm::normalize(glm::cross(camera.direction, camera.up));

            // kameramatrix aufstellen für transformation zw. welt- und lokalem koordinatensystem
            glm::mat4 camera_mat;
            camera_mat[0] = glm::vec4{u, 0.0f};
            camera_mat[1] = glm::vec4{glm::cross(u, (camera.direction)), 0.0f};
            camera_mat[2] = glm::vec4{-(camera.direction), 0.0f};
            camera_mat[3] = glm::vec4{camera.origin, 1.0f};

            // Anti-Aliasing: 1 Pixel in 2x2 Unterpixel unterteilen
            for (float fy = 0.0f; fy < 1.0f; fy += 1.0f / 2.0f) {
                for (float fx = 0.0f; fx < 1.0f; fx += 1.0f / 2.0f) {
                    // Berechnung der strahlenrichtung mit verschiebung für Unterpixel
                    glm::vec4 ray_direction = glm::vec4{glm::normalize(glm::vec3{
                            (-(width_ / 2.0f) + float(p.x + fx)),
                            (-(height_ / 2.0f) + float(p.y + fy)),
                            -d}), 0.0f};

                    ray_direction = glm::normalize(camera_mat * ray_direction);

                    Ray ray = {camera.origin, glm::vec3{ray_direction}};  // Strahl erzeugen
                    Color clr = trace(ray, 0);  // farbe für diesen unterpixel berechnen

                    final_color = final_color + clr;  // Farbe aufsummieren
                }
            }

            // Mittelwert der farben der unterpixel berechnen
            Color cldr = {final_color.r / 4, final_color.g / 4, final_color.b / 4};

            p.color = Color{cldr.r / (cldr.r+1), cldr.g / (cldr.g+1), cldr.b / (cldr.b+1)};

            write(p);  // Schreibe die Farbe des Pixels in das Bild
        }
    }
    ppm_.save(filename_); // Bild speichern in filename_
}

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