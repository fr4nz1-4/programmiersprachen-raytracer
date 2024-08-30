#ifndef RAYTRACER_SHAPE_HPP
#define RAYTRACER_SHAPE_HPP
#include <tuple>
#include <string>
#include "color.hpp"
#include "hitpoint.hpp"
#include "ray.hpp"
#include "material.hpp"
#include "glm/matrix.hpp"

class Shape {
public:
    Shape(std::string const& name, std::shared_ptr<Material> const& material);
    virtual float area() const = 0;
    virtual float volume() const = 0;
    virtual std::ostream& print(std::ostream& os) const;
    virtual HitPoint intersect(Ray const& ray) const = 0;
    virtual glm::vec3 normal(glm::vec3 const& point) const = 0;
    virtual ~Shape();
    std::shared_ptr<Material> get_Material() const;
    std::string get_name() const;
    void set_world_transformation(glm::mat4 const& matrix) ;
    glm::mat4 get_world_transformation() const;
    glm::mat4 get_world_transformation_inv() const ;
//    friend class Scene;
private:
    void compute_inv_world_transform_();

protected:
    std::string name_;
    std::shared_ptr<Material> material_;
    glm::mat4 world_transformation_ = glm::mat4(1.0f);
    glm::mat4 world_transformation_inv = glm::mat4(1.0f);;
};



std::ostream& operator<<(std::ostream& os, Shape const& shape);

#endif //RAYTRACER_SHAPE_HPP
