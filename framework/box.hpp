#ifndef RAYTRACER_BOX_HPP
#define RAYTRACER_BOX_HPP
#include "shape.hpp"
#include <glm/vec3.hpp>
#include "tuple"

class Box : public Shape {
public:
//    Box(glm::vec3 const& min_, glm::vec3 const& max_);
    Box(std::string const& name, std::shared_ptr<Material> const& material, glm::vec3 min, glm::vec3 max);
//    Box();
//    float area() const override;
//    float volume() const override;
    std::ostream& print(std::ostream& os) const override;
    HitPoint intersect (Ray const& ray) const override;
    ~Box();
     virtual glm::vec3 normal(glm::vec3 const& point) const override ;

private:
    glm::vec3 min_;
    glm::vec3 max_;
};

#endif //RAYTRACER_BOX_HPP
