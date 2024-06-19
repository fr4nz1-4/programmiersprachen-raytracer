#ifndef RAYTRACER_SHAPE_HPP
#define RAYTRACER_SHAPE_HPP

class Shape {
public:
    virtual float area() const = 0;
    virtual float volume() const = 0;
};

#endif //RAYTRACER_SHAPE_HPP
