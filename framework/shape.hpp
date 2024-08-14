#ifndef RAYTRACER_SHAPE_HPP
#define RAYTRACER_SHAPE_HPP
#include <tuple>
#include <string>
#include "color.hpp"
#include "hitpoint.hpp"
#include "ray.hpp"
#include "material.hpp"

class Shape {
public:
    Shape(std::string const& name, std::shared_ptr<Material> const& material);
    virtual float area() const = 0;
    virtual float volume() const = 0;
    virtual std::ostream& print(std::ostream& os) const;
    virtual HitPoint intersect(Ray const& ray) const = 0;
    virtual ~Shape();
    std::shared_ptr<Material> get_Material() const;
//    friend class Scene;

protected:
    std::string name_;
    std::shared_ptr<Material> material_;
};

std::ostream& operator<<(std::ostream& os, Shape const& shape);

#endif //RAYTRACER_SHAPE_HPP
