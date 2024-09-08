// erstellt NUR Bilder, schreibt keine sdf dateien
#include <renderer.hpp>
#include <window.hpp>
#include <GLFW/glfw3.h>
#include <thread>
#include "iostream"
#include <fstream>
#include <utility>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//now single threaded again
int main(int argc, char **argv) {
    unsigned const image_width = 1500;
    unsigned const image_height = 1200;

    std::string ordner = "/Users/franziskapobering/repositories/Programmiersprachen/Raytracer/raytracer_images_2/";
    for (int i = 123; i < 125; i++) {
        std::string dateiname = "image" + std::to_string(i) + ".ppm";
        std::string vollstaendigerPfad = ordner + dateiname;
        std::ofstream sdf(vollstaendigerPfad); // hier werden die Bilder abgespeichert

        if (!sdf.is_open()) {
            std::cerr << "Fehler beim öffnen der Datei" << vollstaendigerPfad << std::endl;
        }

        // sdf dateien die nacheinander ausgelesen werden
        std::string filename = "/Users/franziskapobering/repositories/Programmiersprachen/Raytracer/raytracer_sdf_files_2/scene_" + std::to_string(i) + ".sdf";

        if (argc >= 2) {
            filename = argv[1];
        }

        Scene scene1;
        parse_sdf_file(filename, scene1);
        Renderer renderer{image_width, image_height, vollstaendigerPfad, scene1};

        renderer.render(); // rendern und speichern des bildes

//        Window window{{image_width, image_height}};

//        while (!window.should_close()) {
//            if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//                window.close();
//            }
//            window.show(renderer.color_buffer());
//        }

        sdf.close();
    }
}

/*
// schreibt sdf dateien UND erstellt Bilder
#include <renderer.hpp>
#include <window.hpp>
#include <GLFW/glfw3.h>
#include <thread>
#include "iostream"
#include <fstream>
#include <utility>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main(int argc, char **argv) {
    unsigned const image_width = 500;
    unsigned const image_height = 400;

    glm::vec3 current_position;

    // sdf Datei die als Vorlage verwendet wird
    std::ifstream main_source("/Users/franziskapobering/repositories/Programmiersprachen/Raytracer/raytracer_sdf_files_2/scene_00.sdf");

    if (!main_source.is_open()) {
        std::cerr << "Error opening SDF file" << std::endl;
        return -1;
    }

    for (int i = 0; i < 31; ++i) {
        // Bild was gerendert und gespeichert wird
        std::string filename = "/Users/franziskapobering/repositories/Programmiersprachen/Raytracer/raytracer_images_2/image" + std::to_string(i) + ".ppm";
        main_source.seekg(0); // reset the file pointer to the start

        // sdf Datei erstellen
        std::ofstream image_source("/Users/franziskapobering/repositories/Programmiersprachen/Raytracer/raytracer_sdf_files_2/scene_" + std::to_string(i) + ".sdf", std::ios::trunc);

        if (!image_source.is_open()) {
            std::cerr << "Error opening SDF file" << std::endl;
            return -1;
        }

        // szene aus main_source wird in image_source geschrieben
        image_source << main_source.rdbuf();

        if (i >= 0 && i < 41) {
            // bewegung in x und z richtung
            current_position.x = -100.0f + i * 1.25f;
            current_position.z = -150.0f - i * 1.25f;

            // parabel-bewegung auf der y achse
            current_position.y = ((-89.0f / 625.0f) * (current_position.x + 75) * (current_position.x + 75) + 85.0f);

            // transformation der sphere wird in sdf datei geschrieben
            image_source << "\n"
                         << "transform sphere1 rotate 0 1 0 0 translate "
                         << current_position.x << " " << current_position.y << " " << current_position.z
                         << " scale 1 1 1";
        }

        if (i >= 41 && i < 80) {
            // bewegung in x und z richtung
            current_position.x = -50.0f + (i % 40) * 1.25f;
            current_position.z = -200.0f - (i % 40) * 1.25f;


            // parabel-bewegung auf der y achse
            current_position.y = ((-69.0f / 625.0f) * (current_position.x + 25) * (current_position.x + 25) + 40.0f);
            std::cout << current_position.x << ", " << current_position.y << ", " << current_position.z <<std::endl;

            // transformation der sphere wird in sdf datei geschrieben
            image_source << "\n"
                         << "transform sphere1 rotate 0 1 0 0 translate "
                         << current_position.x << " " << current_position.y << " " << current_position.z
                         << " scale 1 1 1";
        }

        if (i == 80) {
            image_source << "\n" << "transform sphere1 rotate 0 1 0 0 translate "
            << 0.0 << " " << -33 << " " << -250
            << " scale 1 1 1";
        }

        if (i >= 81 && i < 122) {
            // bewegung in x und z richtung
            current_position.x = 0.0f + (i % 80) * 1.25f;
            current_position.z = -250.0f - (i % 80) * 1.25f;

            // parabel-bewegung auf der y achse
            current_position.y = ((-49.0f / 625.0f) * (current_position.x - 25) * (current_position.x - 25) + 20.0f);
            std::cout << current_position.x << ", " << current_position.y << ", " << current_position.z <<std::endl;

            // transformation der sphere wird in sdf datei geschrieben
            image_source << "\n"
                         << "transform sphere1 rotate 0 1 0 0 translate "
                         << current_position.x << " " << current_position.y << " " << current_position.z
                         << " scale 1 1 1";
        }

        if (i >= 122 && i < 190) {
            // bewegung in x und z richtung
            current_position.x = 51.25f + (i % 121) * 1.25f;
            current_position.z = -301.25f - (i % 121) * 1.25f;

            // parabel-bewegung auf der y achse
            current_position.y = ((-49.0f / 625.0f) * (current_position.x - 75) * (current_position.x - 75) + 0.0f);
            std::cout << current_position.x << ", " << current_position.y << ", " << current_position.z <<std::endl;

            // transformation der sphere wird in sdf datei geschrieben
            image_source << "\n"
                         << "transform sphere1 rotate 0 1 0 0 translate "
                         << current_position.x << " " << current_position.y << " " << current_position.z
                         << " scale 1 1 1";
        }

        image_source.close();

        // die aktuelle sdf datei wird geparsed
        Scene scene1;
        parse_sdf_file("/Users/franziskapobering/repositories/Programmiersprachen/Raytracer/raytracer_sdf_files_2/scene_" + std::to_string(i) + ".sdf", scene1);

        // Bild wird gerendert
        Renderer renderer{image_width, image_height, filename, scene1};
        renderer.render();
    }
    main_source.close();
    return 0;
} */