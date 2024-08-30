#include "shape.hpp"
#include "iostream"

Shape::Shape(std::string const& name, std::shared_ptr<Material> const& material) :
    name_{name},
    material_{material} {
//    std::cout<< "Shape constructor\n";
}

std::ostream& Shape::print(std::ostream &os) const {
    return os << "name: " + name_ + " | Material: " << material_;
}

Shape::~Shape() {
//    std::cout<< "Shape destructor\n";
}

std::ostream& operator<<(std::ostream& os, Shape const& shape) {
    return shape.print(os);
}

std::shared_ptr<Material> Shape::get_Material() const {
    return material_;
}

std::string Shape::get_name() const {
    return name_;
}

glm::mat4 Shape::get_world_transformation() const {
    return world_transformation_;
}

glm::mat4 Shape::get_world_transformation_inv() const {
    return world_transformation_inv;
}

 void Shape::set_world_transformation(glm::mat4 const& matrix)  {
    world_transformation_ = matrix * world_transformation_;
    compute_inv_world_transform_();
   
}

void Shape::compute_inv_world_transform_() {
    world_transformation_inv = glm::inverse(world_transformation_);
    
}
