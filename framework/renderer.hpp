// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#ifndef BUW_RENDERER_HPP
#define BUW_RENDERER_HPP

#include "color.hpp"
#include "pixel.hpp"
#include "sphere.hpp"
#include "scene.hpp"
#include "ppmwriter.hpp"
#include "camera.hpp"
#include "light.hpp"
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

class Renderer
{
public:
  Renderer(unsigned w, unsigned h, std::string const& file, Scene const& scene);
  void render();
  Color trace(Ray const& ray, int depth);
  Color shade(HitPoint const& closest_object_hitpoint, std::shared_ptr<Shape> const& shape, float distance, Ray const& ray, int depth);
  void write(Pixel const& p);
  Ray transform_ray(glm::mat4 const& mat, Ray const& ray);

        inline std::vector<Color> const& color_buffer() const
  {
    return color_buffer_;
  }

private:
  unsigned width_;
  unsigned height_;
  std::vector<Color> color_buffer_;
  std::string filename_;
  PpmWriter ppm_;
  Scene scene_;
};

#endif // #ifndef BUW_RENDERER_HPP
