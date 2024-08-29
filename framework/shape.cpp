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
    return this->material_;
}

std::string Shape::get_name() const {
    return this->name_;
}

glm::mat4 Shape::get_world_transformation() {
    return this->world_transformation_;
}

glm::mat4 Shape::get_world_transformation_inv() {
    return this->world_transformation_inv;
}

 void Shape::set_world_transformation_(glm::mat4 matrix) {
    world_transformation_ = matrix;
   
}

void Shape::set_world_transformation_inv(glm::mat4 matti) {
    world_transformation_inv = matti;
    
}
