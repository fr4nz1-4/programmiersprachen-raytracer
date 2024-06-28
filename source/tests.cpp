#define CATCH_CONFIG_RUNNER
#include <catch.hpp>
#include "box.hpp"
#include "sphere.hpp"
#include "shape.hpp"
#include "iostream"
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>

TEST_CASE("box area", "[area]") {
    // Normalfall
    Box box1({1, 1, 1}, {3, 3, 3});
    REQUIRE(box1.area() == 24.0f);

    // ein punkt 0
    Box box2({0, 0, 0}, {1, 2, 3});
    REQUIRE(box2.area() == 22.0f);

    // berechnung mit eigentlich negativem ergebnis
    Box box3({3, 6, 1}, {1, 2, 3});
    REQUIRE(box3.area() == 40.0f);
}

TEST_CASE("box volume", "[volume]") {
    // Normalfall
    Box box1{{1, 1, 1}, {3, 3, 3}};
    REQUIRE(box1.volume() == 8.0f);

    // eine seite ist 0
    Box box2({1, 2, 3}, {0, 0, 3});
    REQUIRE(box2.volume() == 0.0f);

    // berechnung mit eigentlich negativem ergebnis
    Box box3({3, 6, 1}, {1, 2, 3});
    REQUIRE(box3.volume() == 16.0f);
}

TEST_CASE("sphere area", "[area]") {
    // Normalfall
    Sphere sphere1({0,0,0}, 5.0f);
    REQUIRE(sphere1.area() == Approx(314.15927f));

    // radius 0
    Sphere sphere2({0,0,0}, 0.0f);
    REQUIRE(sphere2.area() == 0.0f);

    // negativer radius
    Sphere sphere3({0,0,0}, -5.0f);
    REQUIRE(sphere3.area() == Approx(314.15927f));
}

TEST_CASE("sphere volume", "[volume]") {
    // Normalfall
    Sphere sphere1({0,0,0}, 5.0f);
    REQUIRE(sphere1.volume() == Approx(523.59882f));

    // radius 0
    Sphere sphere2({0,0,0}, 0.0f);
    REQUIRE(sphere2.volume() == 0.0f);

    // negativer radius
    Sphere sphere3({0,0,0}, -5.0f);
    REQUIRE(sphere3.volume() == Approx(523.59882f));
}

TEST_CASE("test print") {
    Sphere sphere1({0,0,0}, 5.0f);
    Box box1({1, 1, 1}, {3, 3, 3});
    std::cout<< sphere1;
    std::cout<< box1;
}

TEST_CASE("intersect_ray_sphere", "[intersect]") {
    SECTION("origin im nullpunkt, schneidet kugel") {
        // Ray
        glm::vec3 ray_origin{0.0f, 0.0f, 0.0f};
        // ray direction has to be normalized !
        // you can use:
        // v = glm::normalize(some_vector)
        glm::vec3 ray_direction{0.0f, 0.0f, 1.0f};
        // Sphere
        glm::vec3 sphere_center{0.0f, 0.0f, 5.0f};
        float sphere_radius{1.0f};
        float distance = 0.0f;
        auto result = glm::intersectRaySphere(
                ray_origin, ray_direction,
                sphere_center,
                sphere_radius * sphere_radius, // squared radius !!!
                distance);
        REQUIRE(distance == Approx(4.0f));
    }

    SECTION("ray schneidet kugel nicht") {
        // Ray
        glm::vec3 ray_origin{2.0f, 5.0f, 0.0f};
        // ray direction has to be normalized !
        // you can use:
        // v = glm::normalize(some_vector)
        glm::vec3 ray_direction{0.0f, 0.0f, 1.0f};
        // Sphere
        glm::vec3 sphere_center{0.0f, 2.0f, 3.0f};
        float sphere_radius{2.0f};
        float distance = 0.0f;
        auto result = glm::intersectRaySphere(
                ray_origin , ray_direction ,
                sphere_center ,
                sphere_radius * sphere_radius, // squared radius !!!
                distance);
        REQUIRE(result == false);
    }

    SECTION("origin nicht im nullpunkt, schneidet kugel") {
        // Ray
        glm::vec3 ray_origin{2.0f, 5.0f, 0.0f};
        // ray direction has to be normalized !
        // you can use:
        // v = glm::normalize(some_vector)
        glm::vec3 ray_direction{1.0f, 0.0f, 0.0f};
        // Sphere
        glm::vec3 sphere_center{4.0f, 4.0f, 0.0f};
        float sphere_radius{1.0f};
        float distance = 0.0f;
        auto result = glm::intersectRaySphere(
                ray_origin , ray_direction ,
                sphere_center ,
                sphere_radius * sphere_radius, // squared radius !!!
                distance);
        REQUIRE(result == true);
        REQUIRE(distance == 2.0f);
    }
}

TEST_CASE("5.7: statischer typ, dynamischer typ") {
    Color red{255, 0, 0};
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    std::shared_ptr<Sphere> s1 = std::make_shared<Sphere>("sphere0", red, position, 1.2f);
    std :: shared_ptr < Shape > s2 = std::make_shared<Sphere>("sphere1", red, position, 1.2f);
    s1->print(std::cout);
    s2->print(std::cout);
}

TEST_CASE("test_destructor") {
    Color red{255, 0, 0};
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    Sphere *s1 = new Sphere{"sphere0", red, position, 1.2f};
    Shape *s2 = new Sphere{"sphere1", red, position, 1.2f};
    s1->print(std::cout);
    s2->print(std::cout);
    delete s1;
    delete s2;
}

int main(int argc, char *argv[])
{
  return Catch::Session().run(argc, argv);
}
