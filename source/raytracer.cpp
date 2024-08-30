#include <renderer.hpp>
#include <window.hpp>

#include <GLFW/glfw3.h>
#include <thread>
#include "iostream"
#include <utility>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//now single threaded again
int main(int argc, char** argv)
{
    
  unsigned const image_width = 600;
  unsigned const image_height = 600;
//  std::string const filename = "C:/Users/PC/Desktop/uni/SE/programmiersprachen-raytracer/source/scene_2.sdf";
  //std::string const filename = "C:/Users/PC/Desktop/testi/programmiersprachen-raytracer/source/scene_2.sdf";
  std::string filename = "/Users/franziskapobering/repositories/Programmiersprachen/programmiersprachen-raytracer/source/scene_2.sdf"; // "../../../source/scene_2.sdf";
//  std::string const filename = "/Users/franziskapobering/repositories/Programmiersprachen/programmiersprachen-raytracer/source/test_scene.sdf"; // für test_scene (nur kreis)
//  std::string const filename = "scene_2.sdf";

  if (argc >= 2) {
      filename = argv[1];
  }



  Scene scene1;
  parse_sdf_file(filename, scene1);
  Renderer renderer{image_width, image_height, filename, scene1};

  renderer.render();

  Window window{{image_width, image_height}};

  while (!window.should_close()) {
    if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      window.close();
    }
    window.show(renderer.color_buffer());
  }

  return 0;
}
