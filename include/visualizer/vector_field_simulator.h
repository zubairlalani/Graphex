#ifndef VECTOR_FIELD_GRAPHER_VECTOR_FIELD_SIMULATOR_H
#define VECTOR_FIELD_GRAPHER_VECTOR_FIELD_SIMULATOR_H

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/params/Params.h"
#include <OpenGL/glu.h>
#include "cinder/ImageIo.h"
#include <math.h>

namespace vectorfield {

namespace visualizer {

class FieldSimulator : public ci::app::App {
 public:

  /**
   * Initialize simulation/cinder screen
   */
  FieldSimulator();

  /**
   * Draws all parts of the app onto the cinder window
   * Gets called continuously
   */
  void draw() override;

  /**
   * Updates different parts of backend depending on
   * what is going on in the simulation
   * Note: Gets called before draw() every frame
   */
  void update() override;

  void setup() override;

 private:

  const size_t kWindowSize = 700; // Size of the cinder window
  ci::gl::Texture2dRef texture;


};

} // namespace visualizer
} // namespace vectorfield

#endif  // VECTOR_FIELD_GRAPHER_VECTOR_FIELD_SIMULATOR_H
