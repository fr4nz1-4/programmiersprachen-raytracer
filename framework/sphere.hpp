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
//    Sphere(glm::vec3 const& center, float radius);
    Sphere(std::string const& name,  std::shared_ptr<Material> const& material, glm::vec3 const& center, float radius);
    float area() const override;
    float volume() const override;
    std::ostream& print(std::ostream& os) const override;
    HitPoint intersect (Ray const& ray) const override;
    ~Sphere();

private:
    glm::vec3 center_;
    float radius_;
};

// hello world

#endif //RAYTRACER_SPHERE_HPP
