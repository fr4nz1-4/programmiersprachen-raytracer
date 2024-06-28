#ifndef RAYTRACER_BOX_HPP
#define RAYTRACER_BOX_HPP
#include "shape.hpp"
#include <glm/vec3.hpp>
#include "tuple"

class Box : public Shape {
public:
    float area() const override;
    float volume() const override;
    std::ostream& print(std::ostream& os) const override;
    Box(glm::vec3 min_, glm::vec3 max_);
    Box(const std::string &name, const Color &color, glm::vec3 min, glm::vec3 max);
    ~Box();

private:
    std::string name_;
    Color color_;
    glm::vec3 min_;
    glm::vec3 max_;

};

#endif //RAYTRACER_BOX_HPP
