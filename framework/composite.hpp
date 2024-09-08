#ifndef RAYTRACER_COMPOSITE_HPP
#define RAYTRACER_COMPOSITE_HPP

#include <glm/vec3.hpp>
#include <string>
#include "shape.hpp"
#include <vector>

// zwar teilweise implementiert aber nicht verwendet
class Composite : Shape {
public:
    Composite(std::string const& name);
    HitPoint intersect(Ray const& ray) const override;
    void add_shape(std::shared_ptr<Shape> const& shape);
    glm::vec3 normal(glm::vec3 const& point) const override;

private:
    std::string name_ = " ";
    std::vector<std::shared_ptr<Shape>> children_;
};

#endif //RAYTRACER_COMPOSITE_HPP