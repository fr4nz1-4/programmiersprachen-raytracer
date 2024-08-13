#include <renderer.hpp>
#include <window.hpp>

#include <GLFW/glfw3.h>
#include <thread>
#include <utility>
#include <cmath>

//now single threaded again
int main(int argc, char* argv[])
{
  unsigned const image_width = 600;
  unsigned const image_height = 600;
  std::string const filename = "./checkerboard.ppm";
  //Scene scene1;
  //parse_sdf_file(filename, scene1);
  Renderer renderer{image_width, image_height, filename};
  //auto sphere2 = scene1.shape_container[0];

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
