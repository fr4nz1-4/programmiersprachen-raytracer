#ifndef RAYTRACER_SPHERE_HPP
#define RAYTRACER_SPHERE_HPP
#include "shape.hpp"
#include "string"
#include "tuple"
#include "ray.hpp"
#include "hitpoint.hpp"
#include <glm/vec3.hpp>

class Sphere : public Shape {
public:
    Sphere(glm::vec3 const& center, float radius);
    Sphere(std::string const& name, Color const& color, glm::vec3 const& center, float radius);
    std::ostream& print(std::ostream& os) const override;
    HitPoint intersect (Ray const& ray) const;
    float area() const override;
    float volume() const override;
    ~Sphere();

private:
    std::string name_;
    Color color_;
    glm::vec3 center_;
    float radius_;
};

#endif //RAYTRACER_SPHERE_HPP
