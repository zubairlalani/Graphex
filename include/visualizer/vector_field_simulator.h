#ifndef VECTOR_FIELD_GRAPHER_VECTOR_FIELD_SIMULATOR_H
#define VECTOR_FIELD_GRAPHER_VECTOR_FIELD_SIMULATOR_H

#include <cmath>

#include "../core/function_handler.h"
#include "cinder/ImageIo.h"
#include "cinder/Rand.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/gl.h"
#include "cinder/params/Params.h"
#import "curve_handler.h"
#include "particle_manager.h"
#include "shader.h"
#include "../core/camera.h"
#include "graph_handler.h"
#include "cinder/camera.h"

namespace vectorfield {

namespace visualizer {

using std::string;
using std::vector;

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

  /**
   * Cinder method that sets important parameters up or performs actions
   * that only needed to be performed once
   * Ex: creating button panel
   */
  void setup() override;

  /**
   * Takes care of events that occur when the mouse is dragged
   * @param event - mouse event
   */
  void mouseDrag(ci::app::MouseEvent event) override;

  /**
   * Events that occur when mouse is released
   * @param event - mouse event
   */
  void mouseUp(ci::app::MouseEvent event) override;

  void mouseDown(ci::app::MouseEvent event) override;

  void keyDown(ci::app::KeyEvent event) override;

  void mouseWheel(ci::app::MouseEvent event) override;

 private:

  const size_t kWindowSize = 700; // Size of the cinder window
  const int kScale = 10; // Determines how many units an axis goes up to
  int k3DScale = 5;
  const int kVectorScale = 10; // Number of vectors that will be drawn
  const float kArrowHeight = 3.0f; // Height of arrowhead
  const float kArrowBase = 10.0f; // Distance that the base of the arrow spans
  const std::vector<ci::Color> kPenColors = {
      ci::Color("red"),
      ci::Color("green"),
      ci::Color("blue"),
      ci::Color("white")
  };

  /**
  * Performs an actions depending on which button is clicked
  * Drawing vector field, clearing vector field, etc
  * @param id - indicates which button is being pressed
  */
  void button(size_t id);

  void ClearArrows();

  void InitializeArrowVertices(int x, int y, const glm::vec2& velocity);

  void Initialize3DArrowVertices();

  void DrawFPS();

  void DrawMouseCoordinates();

  void DrawCalculations();

  void InitializeFieldVectors();

  void SetupTweakBar();

  void InitializeBatch();

  void InitializeBuffers(const std::vector<float>& vertices);

  void CalculateMVPMatrix();

  ci::Color GetNextColor();

  size_t color_index_ = 0; // Determines where in pen color array we are in

  ci::params::InterfaceGlRef params_gui_; // Used for user input

  string i_component_; // i-component of the vector field that user provided
  string j_component_; // j-component of the vector field that user provided
  string k_component_; // k-component of the vector field that user provided

  string x_pos_; // x position of particle that user inputted
  string y_pos_; // y position of particle that user inputted

  ParticleManager particle_manager_; // Draws and updates all particles on screen
  float image_scaling_factor_ = .1f; // Relative size of the arrows

  FunctionHandler function_handler_; // Computes all math involved
  GraphHandler graph_handler_;

  glm::vec2 mouse_pos_;
  bool left_down_;
  bool in_range_;

  ci::gl::GlslProgRef glsl_prog_;
  ci::gl::BatchRef arrow_batch_;
  ci::gl::VertBatchRef      vertBatch;

  CurveHandler curve_handler_;
  bool pen_mode_ = false;

  double total_work_;
  double divergence_;
  double curl_;
  bool conservative = false;

  std::string equation_;  // Equation that the user inputs for graph 1
  std::string equation2_; // Equation that the user inputs for graph 2

  //3D stuff
  Shader shader_ = Shader("visualizer/shader.vs", "visualizer/shader.fs");
  unsigned int vbo_, vao_;
  glm::mat4 model, view, projection;

  //Axis stuff
  std::vector<float> axisvertices = {
      0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 0.0f,
      0.0f, -5.0f, 0.0f, 1.0f, 1.0f, 0.0f,
      5.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
      -5.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 5.0f, 1.0f, 1.0f, 0.0f,
      0.0f, 0.0f, -5.0f, 1.0f, 1.0f, 0.0f
  };

  Camera camera = Camera(glm::vec3(0, 0, 13.0f));
  bool first_mouse_ = true;

  glm::vec2 mousePos = glm::vec2(0, 0);

  bool toggle3d = false;

  size_t vertex_amnt_;

  ci::Color arrow_color_ = ci::Color(0, 1, 6);
  ci::Color axes_color_ = ci::Color(1, 1, 0);

};

} // namespace visualizer
} // namespace vectorfield

#endif  // VECTOR_FIELD_GRAPHER_VECTOR_FIELD_SIMULATOR_H
