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
     world_transformation_ = matrix; //* world_transformation_;
    compute_inv_world_transform_();
}

void Shape::compute_inv_world_transform_() {
    world_transformation_inv = glm::inverse(world_transformation_);
}

void Shape::make_world_transform(glm::vec3 const& scale, glm::vec3 const& translate, float rot_degree, glm::vec3 const& rotate) {
//    std::cout << "Parameters for world transformation:" << std::endl;
//    std::cout << "Scale: (" << scale.x << ", " << scale.y << ", " << scale.z << ")" << std::endl;
//    std::cout << "Translate: (" << translate.x << ", " << translate.y << ", " << translate.z << ")" << std::endl;
//    std::cout << "Rotation angle (degrees): " << rot_degree << std::endl;
//    std::cout << "Rotation axis: (" << rotate.x << ", " << rotate.y << ", " << rotate.z << ")" << std::endl;

    glm::mat4 translate_m = glm::translate(glm::mat4(1.0f), translate);
    glm::mat4 scale_m = glm::scale(glm::mat4(1.0f), scale);
    glm::vec3 normalized_rotate = glm::normalize(rotate);
    glm::mat4 rotation_m = glm::rotate(glm::mat4(1.0f), glm::radians(rot_degree), normalized_rotate);
    glm::mat4 world_transform = translate_m * rotation_m * scale_m;
//    std::cout << "rot:\n" << glm::to_string(rotation_m) << std::endl;

    set_world_transformation(world_transform);
//    std::cout << "world_transform:\n" << glm::to_string(get_world_transformation()) << std::endl;
}