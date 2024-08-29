#define CATCH_CONFIG_RUNNER
#include <catch.hpp>
#include "box.hpp"
//#include "sphere.hpp"
#include "shape.hpp"
#include "iostream"
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include "scene.hpp"
/*

//Material blue = {"blue", {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, 20.0f};
//Material green = {"green", {0, 0, 1}, {0, 0, 1}, {0, 0, 1}, 20.0f};
Material red = {"red", Color{1, 0, 0}, Color{1, 0, 0}, Color{1, 0, 0}, 20.0f};
std::shared_ptr<Material> red = std::make_shared<Material>(red);

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

        Box box = {"test", red, {1.0f,3.0f,5.0f}, {5.0f,4.0f,3.0f}};
        Sphere sphere = {"test", red, {4,3,2}, 3.4f};
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

TEST_CASE("konstruktor destruktor Reihenfolge") {
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    Sphere *s1 = new Sphere{"sphere0", red, position, 1.2f};
    Shape *s2 = new Sphere{"sphere1", red, position, 1.2f};
    s1->print(std::cout);
    s2->print(std::cout);
    delete s1; // if destructor isn't virtual: s1 is objekt of class shape
    delete s2;
}
//

TEST_CASE("intersect_ray_box", "[intersect]") {
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
        std::cout<<b1;

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
*/


TEST_CASE("parse_sdf_file correctly parses materials", "[parse_sdf_file]") {
// leere scene erstellen um sie zu übergeben
    Scene scene;

//    parse_sdf_file("/Users/franziskapobering/repositories/Programmiersprachen/programmiersprachen-raytracer/scene_2.sdf", scene);
//    parse_sdf_file("./scene_2.sdf", scene);
    parse_sdf_file("../scene_2.sdf", scene);

    REQUIRE(scene.material_container.size() == 3);
    REQUIRE(scene.shape_container.size() == 2);

// Test für material red
    auto red_material= scene.material_container[0];
    REQUIRE(red_material->name_ == "red");
    REQUIRE(red_material->ka.r == 1.0f);
    REQUIRE(red_material->ka.g == 0.0f);
    REQUIRE(red_material->ka.b == 0.0f);
    REQUIRE(red_material->kd.r == 1.0f);
    REQUIRE(red_material->kd.g == 0.0f);
    REQUIRE(red_material->kd.b == 0.0f);
    REQUIRE(red_material->ks.r == 1.0f);
    REQUIRE(red_material->ks.g == 0.0f);
    REQUIRE(red_material->ks.b == 0.0f);
    REQUIRE(red_material->m == 20);

// Test für material green
    auto green_material= scene.material_container[1];
    REQUIRE(green_material->name_ == "green");
    REQUIRE(green_material->ka.r == 0.0f);
    REQUIRE(green_material->ka.g == 1.0f);
    REQUIRE(green_material->ka.b == 0.0f);
    REQUIRE(green_material->kd.r == 0.0f);
    REQUIRE(green_material->kd.g == 1.0f);
    REQUIRE(green_material->kd.b == 0.0f);
    REQUIRE(green_material->ks.r == 0.0f);
    REQUIRE(green_material->ks.g == 1.0f);
    REQUIRE(green_material->ks.b == 0.0f);
    REQUIRE(green_material->m == 50);

// Test für material blue
    auto blue_material = scene.material_container[2];
    REQUIRE(blue_material->name_ == "blue");
    REQUIRE(blue_material->ka.r == 0.0f);
    REQUIRE(blue_material->ka.g == 0.0f);
    REQUIRE(blue_material->ka.b == 1.0f);
    REQUIRE(blue_material->kd.r == 0.0f);
    REQUIRE(blue_material->kd.g == 0.0f);
    REQUIRE(blue_material->kd.b == 1.0f);
    REQUIRE(blue_material->ks.r == 0.0f);
    REQUIRE(blue_material->ks.g == 0.0f);
    REQUIRE(blue_material->ks.b == 1.0f);
    REQUIRE(blue_material->m == 10);

// test box
//    auto box1  = scene.shape_container[0];
    auto box1 = std::dynamic_pointer_cast<Box>(scene.shape_container[0]);
    REQUIRE(scene.shape_container.empty() == false);
    // box wird korrekt eingelesen wenn alle variablen der klassen public
    /*
    REQUIRE(box1->name_ == "box1");
    REQUIRE(box1->material_->name_ == "red");
    REQUIRE(box1->material_->ka.r == 1.0f);
    REQUIRE(box1->material_->ka.g == 0.0f);
    REQUIRE(box1->material_->ka.b == 0.0f);
    REQUIRE(box1->material_->kd.r == 1.0f);
    REQUIRE(box1->material_->kd.g == 0.0f);
    REQUIRE(box1->material_->kd.b == 0.0f);
    REQUIRE(box1->material_->ks.r == 1.0f);
    REQUIRE(box1->material_->ks.g == 0.0f);
    REQUIRE(box1->material_->ks.b == 0.0f);
    REQUIRE(box1->material_->m == 20.0f);
    REQUIRE(box1->min_.x == 3.0f);
    REQUIRE(box1->min_.y == 1.0f);
    REQUIRE(box1->min_.z == 1.0f);
    REQUIRE(box1->max_.x == 2.0f);
    REQUIRE(box1->max_.y == 2.0f);
    REQUIRE(box1->max_.z == 2.0f);
    */

    // test sphere
    auto sphere1 = std::dynamic_pointer_cast<Sphere>(scene.shape_container[1]);
    REQUIRE(scene.shape_container.empty() == false);
    // sphere wird korrekt eingelesen wenn alle variablen der klassen public
    /*
    REQUIRE(sphere1->name_ == "sphere1");
    REQUIRE(sphere1->material_->name_ == "blue");
    REQUIRE(sphere1->material_->ka.r == 0.0f);
    REQUIRE(sphere1->material_->ka.g == 0.0f);
    REQUIRE(sphere1->material_->ka.b == 1.0f);
    REQUIRE(sphere1->material_->kd.r == 0.0f);
    REQUIRE(sphere1->material_->kd.g == 0.0f);
    REQUIRE(sphere1->material_->kd.b == 1.0f);
    REQUIRE(sphere1->material_->ks.r == 0.0f);
    REQUIRE(sphere1->material_->ks.g == 0.0f);
    REQUIRE(sphere1->material_->ks.b == 1.0f);
    REQUIRE(sphere1->material_->m == 10.0f);
    REQUIRE(sphere1->center_.x == 2.0f);
    REQUIRE(sphere1->center_.y == 5.0f);
    REQUIRE(sphere1->center_.z == 2.0f);
    REQUIRE(sphere1->radius_ == 10.0f);
    */
}

TEST_CASE("matrix tarnslation test", "[]") {
    glm::mat4 matrix{1};
    glm::mat4 matrix2 = {1, 0, 0, 5, 0, 1, 0, 5, 0, 0, 1, 5, 0, 0, 0, 1};
    glm::vec3 vec{5, 5, 5};
    matrix = glm::translate(matrix, vec);
    REQUIRE(matrix[0][0] == matrix2[0][0]);
    REQUIRE(matrix[0][1] == matrix2[0][1]);
    REQUIRE(matrix[0][2] == matrix2[0][2]);
    REQUIRE(matrix[0][3] == matrix2[0][3]);
    REQUIRE(matrix[1][0] == matrix2[1][0]);
    REQUIRE(matrix[1][1] == matrix2[1][1]);
    REQUIRE(matrix[1][2] == matrix2[1][2]);
    REQUIRE(matrix[1][3] == matrix2[1][3]);
    REQUIRE(matrix[2][0] == matrix2[2][0]);
    REQUIRE(matrix[2][1] == matrix2[2][1]);
    REQUIRE(matrix[2][2] == matrix2[2][2]);
    REQUIRE(matrix[2][3] == matrix2[2][3]);
    REQUIRE(matrix[3][0] == matrix2[3][0]);
    REQUIRE(matrix[3][1] == matrix2[3][1]);
    REQUIRE(matrix[3][2] == matrix2[3][2]);
    REQUIRE(matrix[3][3] == matrix2[3][3]);
}
int main(int argc, char *argv[])
{
  return Catch::Session().run(argc, argv);
}
