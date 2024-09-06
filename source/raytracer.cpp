
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
    unsigned const image_width = 600;
    unsigned const image_height = 600;

    std::string ordner = "/Users/franziskapobering/repositories/Programmiersprachen/Raytracer/raytracer_images/";
    for (int i = 0; i < 120; i++) {
        std::string dateiname = "image" + std::to_string(i) + ".ppm";
        std::string vollstaendigerPfad = ordner + dateiname;
        std::ofstream sdf(vollstaendigerPfad);

        if (!sdf.is_open()) {
            std::cerr << "Fehler beim öffnen der Datei" << vollstaendigerPfad << std::endl;
        }

//  std::string const filename = "C:/Users/PC/Desktop/uni/SE/programmiersprachen-raytracer/source/scene_2.sdf";
        //std::string const filename = "C:/Users/PC/Desktop/testi/programmiersprachen-raytracer/source/scene_2.sdf";
//  std::string filename = "/Users/franziskapobering/repositories/Programmiersprachen/programmiersprachen-raytracer/source/scene_2.sdf"; // "../../../source/scene_2.sdf";
        std::string filename = "/Users/franziskapobering/repositories/Programmiersprachen/Raytracer/raytracer_sdf_files/scene_" + std::to_string(i) + ".sdf";
//std::string filename = "../../source/test_scene.sdf";

        if (argc >= 2) {
            filename = argv[1];
        }

        Scene scene1;
        parse_sdf_file(filename, scene1);
        Renderer renderer{image_width, image_height, vollstaendigerPfad, scene1};

        renderer.render();

//        Window window{{image_width, image_height}};
//
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

// Funktion zur Berechnung der Parabelbewegung
glm::vec3 calculate_parabola(float t, glm::vec3 start, glm::vec3 end, float max_height) {
    // Lineare Interpolation für x und z
    float x = start.x + (end.x - start.x) * t;
    float z = start.z + (end.z - start.z) * t;

    // Parabolische Interpolation für y
    float y = (4 * max_height) * t * (1 - t); // Parabolisches y

    return glm::vec3(x, y, z);
}

int main(int argc, char **argv) {
    unsigned const image_width = 600;
    unsigned const image_height = 600;
    int total_frames = 120;
    float fps = 24.0f;
    float time_per_jump = 2.0f; // in Sekunden
    int frames_per_jump = time_per_jump * fps;

    // Box-Positionen
    glm::vec3 box1_pos(-50, 0, -200);
    glm::vec3 box5_pos(0, 0, -250);
    glm::vec3 box9_pos(50, 0, -300);

    // Maximale Höhe für die parabolische Bewegung
    float max_height = 50.0f;

    // Verzeichnis für Bildausgabe
    std::string ordner = "/Users/franziskapobering/repositories/Programmiersprachen/Raytracer/raytracer_images/";

    // Verzeichnis für die SDF-Dateien
    std::string sdf_dir = "/Users/franziskapobering/repositories/Programmiersprachen/Raytracer/raytracer_sdf_files/";

    for (int i = 0; i < total_frames; i++) {
        // Berechne den Sprungabschnitt
        int jump_phase = (i / frames_per_jump) % 3;  // Bestimme, in welchem Sprungabschnitt sich die Kugel befindet
        float t = (i % frames_per_jump) / (float)frames_per_jump;  // Wert zwischen 0 und 1 für Interpolation

        glm::vec3 sphere_position;
        if (jump_phase == 0) {
            sphere_position = calculate_parabola(t, box1_pos, box5_pos, max_height);
        } else if (jump_phase == 1) {
            sphere_position = calculate_parabola(t, box5_pos, box9_pos, max_height);
        } else {
            sphere_position = glm::vec3(50, 0, -300); // Kugel bleibt bei box9
        }

        // Lade die SDF-Datei
        std::string sdf_filename = sdf_dir + "scene_" + std::to_string(i) + ".sdf";
        std::ofstream sdf(sdf_filename);
        if (!sdf.is_open()) {
            std::cerr << "Fehler beim Öffnen der SDF-Datei: " << sdf_filename << std::endl;
            continue;
        }

        // Schreibe die SDF-Datei mit der aktualisierten Kugelposition
        sdf << "# materials\n";
        sdf << "define material red 1 0 0 1 0 0 1 0 0 20\n";
        sdf << "define material green 0 1 0 0 1 0 0 1 0 1000\n";
        sdf << "define material blue 0 0 1 0 0 1 0 0 1 10\n";
        sdf << "define material gray 0.5 0.5 0.5 0.5 0.5 0.5 0.5 0.5 0.5 10\n";
        sdf << "# shapes\n";
        sdf << "define shape box box0 gray -25 -2 -25 25 2 25\n";
        sdf << "define shape box box1 red -25 -2 -25 25 2 25\n";
        sdf << "define shape box box2 green -25 -2 -25 25 2 25\n";
        sdf << "define shape box box3 blue -25 -2 -25 25 2 25\n";
        sdf << "define shape box box4 green -25 -2 -25 25 2 25\n";
        sdf << "define shape box box5 blue -25 -2 -25 25 2 25\n";
        sdf << "define shape box box6 red -25 -2 -25 25 2 25\n";
        sdf << "define shape box box7 blue -25 -2 -25 25 2 25\n";
        sdf << "define shape box box8 red -25 -2 -25 25 2 25\n";
        sdf << "define shape box box9 green -25 -2 -25 25 2 25\n";
        sdf << "define shape sphere sphere1 gray 0 0 0 20\n";
        sdf << "# light postion color brightness\n";
        sdf << "define light light1 0.0 200.0 -250.0 1.0 1.0 1.0 1.0\n";
        sdf << "define light light2 -30.0 0.0 50.0 1.0 1.0 1.0 1.0\n";
        sdf << "# camera\n";
        sdf << "define camera camera1 45.0 0.0 0.0 0.0 0.0 0.0 -1.0 0.0 1.0 0.0\n";
        sdf << "# transformations\n";
        sdf << "transform box0 rotate 0 1 0 0 translate -100 -50 -150 scale 1 1 1\n";
        sdf << "transform box1 rotate 0 1 0 0 translate -50 -50 -200 scale 1 1 1\n";
        sdf << "transform box2 rotate 0 1 0 0 translate 0 -50 -200 scale 1 1 1\n";
        sdf << "transform box3 rotate 0 1 0 0 translate 50 -50 -200 scale 1 1 1\n";
        sdf << "transform box4 rotate 0 1 0 0 translate -50 -50 -250 scale 1 1 1\n";
        sdf << "transform box5 rotate 0 1 0 0 translate 0 -50 -250 scale 1 1 1\n";
        sdf << "transform box6 rotate 0 1 0 0 translate 50 -50 -250 scale 1 1 1\n";
        sdf << "transform box7 rotate 0 1 0 0 translate -50 -50 -300 scale 1 1 1\n";
        sdf << "transform box8 rotate 0 1 0 0 translate 0 -50 -300 scale 1 1 1\n";
        sdf << "transform box9 rotate 0 1 0 0 translate 50 -50 -300 scale 1 1 1\n";
        sdf << "transform sphere1 translate " << sphere_position.x << " " << sphere_position.y << " " << sphere_position.z << " scale 1 1 1\n";

        sdf.close();

        // Bilddateinamen und Pfad erstellen
        std::string dateiname = "image" + std::to_string(i) + ".ppm";
        std::string vollstaendigerPfad = ordner + dateiname;

        // Szene parsen und rendern
        Scene scene1;
        parse_sdf_file(sdf_filename, scene1);
        Renderer renderer{image_width, image_height, vollstaendigerPfad, scene1};
        renderer.render();
    }

    return 0;
}*/