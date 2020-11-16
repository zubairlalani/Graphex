#ifndef VECTOR_FIELD_GRAPHER_VECTOR_FIELD_SIMULATOR_H
#define VECTOR_FIELD_GRAPHER_VECTOR_FIELD_SIMULATOR_H

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/params/Params.h"
#include <OpenGL/glu.h>
#include "cinder/ImageIo.h"
#include <math.h>
#include <exprtk.hpp>

//#include "/Users/zubairlalani/Desktop/cinder_0.9.2_mac/blocks/Cinder-UI/src/UI.h"
//#include </Users/zubairlalani/Desktop/cinder_0.9.2_mac/blocks/PretzelGui/src/Resource.h>
//#include <../blocks/PretzelGui/src/pretzel/PretzelGui.h>


namespace vectorfield {

namespace visualizer {

//using namespace reza::ui;

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

  void button(size_t id);

 private:

  const size_t kWindowSize = 700; // Size of the cinder window
  const size_t kGraphMargin = 50;
  const size_t kInputBoxHeight = 100;
  //std::vector<ci::mat4> matrices_;
  //ci::gl::Texture2dRef texture;
  //ci::TextBox textBox;
  /*
  SuperCanvasRef mUi;
  float mRed = 0.0;
  float mGreen = 0.0;
  float mBlue = 0.0;*/
  //pretzel::PretzelGuiRef gui;
  //ci::glm::vec2 mPosition;
  ci::gl::Texture2dRef mTextTexture;
  ci::params::InterfaceGlRef mParams;
  std::string x_component_;

  exprtk::symbol_table<double> table;
  exprtk::expression<double> expr;
  exprtk::parser<double> parser;
  const std::string expression = "x + y^2 + 3*6";

  //exprtk::symbol_table<std::string> symbolTable;

};

} // namespace visualizer
} // namespace vectorfield

#endif  // VECTOR_FIELD_GRAPHER_VECTOR_FIELD_SIMULATOR_H
