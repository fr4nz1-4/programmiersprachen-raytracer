#define CATCH_CONFIG_RUNNER
#include <catch.hpp>
#include "box.hpp"
#include "sphere.hpp"
#include "shape.hpp"

TEST_CASE("box area", "[area]") {
    // Normalfall
    Box box1 ({1,1,1}, {3,3,3});
    REQUIRE(box1.area() == 24.0f);

    // ein punkt 0
    Box box2 ({0,0,0}, {1,2,3});
    REQUIRE(box2.area() == 22.0f);

    // berechnung mit eigentlich negativem ergebnis
    Box box3 ({3,6,1}, {1,2,3});
    REQUIRE(box3.area() == 40.0f);
}

TEST_CASE("box volume", "[volume]") {
    // Normalfall
    Box box1 ({1,1,1}, {3,3,3});
    REQUIRE(box1.volume() == 8.0f);

    // eine seite ist 0
    Box box2 ({1,2,3}, {0,0,3});
    REQUIRE(box2.volume() == 0.0f);

    // berechnung mit eigentlich negativem ergebnis
    Box box3 ({3,6,1}, {1,2,3});
    REQUIRE(box3.volume() == 40.0f);
}

TEST_CASE("sphere area", "[area]") {
    // Normalfall
    Sphere sphere1({0,0,0}, 5.0f);
    REQUIRE(sphere1.area() == Approx(314.0f));

    // radius 0
    Sphere sphere2({0,0,0}, 0.0f);
    REQUIRE(sphere2.area() == 0.0f);

    // negativer radius
    Sphere sphere3({0,0,0}, -5.0f);
    REQUIRE(sphere3.area() == Approx(314.0f));
}

TEST_CASE("sphere volume", "[volume]") {
    // Normalfall
    Sphere sphere1({0,0,0}, 5.0f);
    REQUIRE(sphere1.volume() == Approx(523.0f));

    // radius 0
    Sphere sphere2({0,0,0}, 0.0f);
    REQUIRE(sphere2.volume() == 0.0f);

    // negativer radius
    Sphere sphere3({0,0,0}, -5.0f);
    REQUIRE(sphere3.volume() == Approx(523.0f));
}

int main(int argc, char *argv[])
{
  return Catch::Session().run(argc, argv);
}
