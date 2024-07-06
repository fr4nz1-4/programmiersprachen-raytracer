#define CATCH_CONFIG_RUNNER
#include <catch.hpp>
#include "box.hpp"
//#include "sphere.hpp"
#include "shape.hpp"
#include "iostream"
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>

//Material blue = {"blue", {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, 20.0f};
//Material green = {"green", {0, 0, 1}, {0, 0, 1}, {0, 0, 1}, 20.0f};
Material rot = {"red", Color{1, 0, 0}, Color{1, 0, 0}, Color{1, 0, 0}, 20.0f};
std::shared_ptr<Material> red = std::make_shared<Material>(rot);

/*
TEST_CASE("area_tests", "[area]") {
    SECTION("box area") {
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
    SECTION("sphere area") {
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
}

TEST_CASE("volume_tests", "[volume]") {
    SECTION("box volume") {
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
    SECTION("Sphere volume") {
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
}

TEST_CASE("print_test") {
    Sphere sphere1({0,0,0}, 5.0f);
    Box box1({1, 1, 1}, {3, 3, 3});
    std::cout<< sphere1;
    std::cout<< box1;
}
*/

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
        REQUIRE(result == true);
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

/*
TEST_CASE("5.7: statischer typ, dynamischer typ") {
    Color red{255, 0, 0};
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    std::shared_ptr<Sphere> s1 = std::make_shared<Sphere>("sphere0", red, position, 1.2f);
    std :: shared_ptr < Shape > s2 = std::make_shared<Sphere>("sphere1", red, position, 1.2f);
    s1->print(std::cout);
    s2->print(std::cout);
}

TEST_CASE("konstruktor destruktor Reihenfolge") {
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    Sphere *s1 = new Sphere{"sphere0", red, position, 1.2f};
    Shape *s2 = new Sphere{"sphere1", red, position, 1.2f};
    s1->print(std::cout);
    s2->print(std::cout);
    delete s1; // if destructor isn't virtual: s1 is objekt of class shape
    delete s2;
}
*/

TEST_CASE("intersect_ray_box", "[intersect]") {
    /*
    Box r1{ "red1", red, glm::vec3 {-1.0f,-1.0f,6.0f}, glm::vec3 {1.0f,1.0f,7.0f}};
    Box y1{ "yellow1", red, glm::vec3 {0.0f,4.0f,0.0f}, glm::vec3 {1.0f,5.0f,1.0f}};
    Box g1{ "green1", red, glm::vec3 {4.0f,-1.0f,0.0f}, glm::vec3 {5.0f,1.0f,1.0f}};

    Ray x{ glm::vec3{0,0,0} , glm::vec3{1,0,0} };
    Ray y{ glm::vec3{0,0,0} , glm::vec3{0,1,0} };
    Ray z{ glm::vec3{0,0,0} , glm::vec3{0,0,1} };
    Ray mz{ glm::vec3{0,0,0} , glm::vec3{0,0,-1} };

    HitPoint r1_mz = r1.intersect(mz);
    HitPoint r1_x = r1.intersect(x);
    HitPoint r1_y = r1.intersect(y);
    HitPoint r1_z = r1.intersect(z);
    std::cout << "\n";

    HitPoint y1_x = y1.intersect(x);
    HitPoint y1_y = y1.intersect(y);
    HitPoint y1_z = y1.intersect(z);
    HitPoint y1_mz = y1.intersect(mz);
    std::cout << "\n";

    HitPoint g1_x = g1.intersect(x);
    HitPoint g1_y = g1.intersect(y);
    HitPoint g1_z = g1.intersect(z);
    HitPoint g1_mz = g1.intersect(mz);
    std::cout << "\n";

    REQUIRE(r1_x.intersection == false);
    REQUIRE(r1_y.intersection == false);
    REQUIRE(r1_mz.intersection == false);
    REQUIRE(y1_x.intersection == false);
    REQUIRE(y1_z.intersection == false);
    REQUIRE(y1_mz.intersection == false);
    REQUIRE(g1_z.intersection == false);
    REQUIRE(g1_y.intersection == false);
    REQUIRE(g1_mz.intersection == false);

    REQUIRE(r1_z.intersection == true);
    REQUIRE(r1_z.distance == 6.0f);
    REQUIRE(r1_z.direction == glm::vec3{ 0,0,1 });
    REQUIRE(r1_z.name == std::string{ "red1" });
    REQUIRE(r1_z.intersection_point == glm::vec3{ 0,0,6 });

    REQUIRE(y1_y.intersection == true);
    REQUIRE(y1_y.distance == 4.0f);
    REQUIRE(y1_y.direction == glm::vec3{ 0,1,0 });
    REQUIRE(y1_y.name == std::string{ "yellow1" });
    REQUIRE(y1_y.intersection_point == glm::vec3{ 0,4,0 });

    REQUIRE(g1_x.intersection == true);
    REQUIRE(g1_x.distance == 4.0f);
    REQUIRE(g1_x.direction == glm::vec3{ 1,0,0 });
    REQUIRE(g1_x.name == std::string{ "green1" });
    REQUIRE(g1_x.intersection_point == glm::vec3{ 4,0,0 });
*/

    Box b1{"box1", red, {2,0,0}, {4,1,1}}; // rand-/kantenfall
    Box b2{"box2", red, {-1,4,-1}, {3,8,3}};
    Box b3{"box3", red, {-2,-1,6}, {1,2,9}};
//    Box b4{"box4", red, {-6,-4,2}, {-3,-1,5}};

    Ray ray_x{{0,0,0}, {1,0,0}};
    Ray ray_y{{0,0,0}, {0,1,0}};
    Ray ray_z{{0,0,0}, {0,0,1}};
    Ray ray_nx{{0,-2,0}, {-2,-2,0}}; // strahl nicht aus ursprung

    SECTION("b1_test") {
        HitPoint b1_ray_x = b1.intersect(ray_x);
        HitPoint b1_ray_y = b1.intersect(ray_y);
        HitPoint b1_ray_z = b1.intersect(ray_z);
        HitPoint b1_ray_nx = b1.intersect(ray_nx);
        std::cout << "\n";

        REQUIRE(b1_ray_x.intersection == true);
        REQUIRE(b1_ray_x.distance == 2.0f);
        REQUIRE(b1_ray_x.intersection_point == glm::vec3{2,0,0});
        REQUIRE(b1_ray_x.name == "box1");

        REQUIRE(b1_ray_y.intersection == false);
        REQUIRE(b1_ray_z.intersection == false);

        REQUIRE(b1_ray_nx.intersection == false);
    }

    SECTION("b2_test") {
        HitPoint b2_ray_x = b2.intersect(ray_x);
        HitPoint b2_ray_y = b2.intersect(ray_y);
        HitPoint b2_ray_z = b2.intersect(ray_z);
        HitPoint b2_ray_nx = b1.intersect(ray_nx);
        std::cout << "\n";

        REQUIRE(b2_ray_y.intersection == true);
        REQUIRE(b2_ray_y.distance == 4.0f);
        REQUIRE(b2_ray_y.intersection_point == glm::vec3{0,4,0});
        REQUIRE(b2_ray_y.name == "box2");

        REQUIRE(b2_ray_x.intersection == false);
        REQUIRE(b2_ray_z.intersection == false);
        REQUIRE(b2_ray_nx.intersection == false);
    }

    SECTION("b3_test") {
        HitPoint b3_ray_x = b3.intersect(ray_x);
        HitPoint b3_ray_y = b3.intersect(ray_y);
        HitPoint b3_ray_z = b3.intersect(ray_z);
        HitPoint b3_ray_nx = b1.intersect(ray_nx);
        std::cout << "\n";

        REQUIRE(b3_ray_z.intersection == true);
        REQUIRE(b3_ray_z.distance == 6.0f);
        REQUIRE(b3_ray_z.intersection_point == glm::vec3{0,0,6});

        REQUIRE(b3_ray_x.intersection == false);
        REQUIRE(b3_ray_y.intersection == false);
        REQUIRE(b3_ray_nx.intersection == false);
    }
}
int main(int argc, char *argv[])
{
  return Catch::Session().run(argc, argv);
}
