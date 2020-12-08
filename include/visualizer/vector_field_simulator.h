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

  /**
   * Events that occur when the mouse is clicked
   * @param event - mouse event
   */
  void mouseDown(ci::app::MouseEvent event) override;

  /**
   * Events that occur when a key is pressed
   * @param event - key event
   */
  void keyDown(ci::app::KeyEvent event) override;

  /**
   * Zoom functionality when the mouse wheel is scrolled
   * @param event
   */
  void mouseWheel(ci::app::MouseEvent event) override;

 private:

  const size_t kWindowSize = 700; // Size of the cinder window
  const int kScale = 10; // Determines how many units an axis goes up to
  int k3DScale = 5; // How many units in every direction the 3D axes span
  const int kVectorScale = 10; // Number of vectors that will be drawn
  const float kArrowHeight = 3.0f; // Height of arrowhead
  const float kArrowBase = 10.0f; // Distance that the base of the arrow spans
  const std::vector<ci::Color> kPenColors = { // Different color options for drawing
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

  /**
   * Clears all arrows from the screen
   */
  void ClearArrows();

  /**
   * Using the starting position of a single arrow and its velocity, adds
   * vertices to the batch that will be connected into lines to draw an arrow
   * @param x - starting x position of the arrow
   * @param y - starting y position of the arrow
   * @param velocity - velocity of the arrow - determines length and direction of arrow
   */
  void InitializeArrowVertices(int x, int y, const glm::vec2& velocity);

  /**
   * Goes through every point in 3D coordinate system where a field vector
   * will be drawn and calculates what vertices are needed in order
   * to draw the field lines
   */
  void Initialize3DArrowVertices();

  /**
   * Draws current FPS
   */
  void DrawFPS();

  /**
   * Draw the current mouse screen position coordinates
   */
  void DrawMouseCoordinates();

  /**
   * Displays divergence, curl, work
   */
  void DrawCalculations();

  /**
   * Goes through every coordinate where a vector will be drawn
   * and evaluates the velocity in order to create vertices
   */
  void InitializeFieldVectors();

  /**
   * Sets up gui with buttons and text boxes
   */
  void SetupTweakBar();

  /**
   * Initializes all batches in order to use batch rendering
   */
  void InitializeBatch();

  /**
   * Initialize the buffers for drawing 3D and how to interpret their data
   * (vertex position and color attributes, etc)
   * @param vertices
   */
  void InitializeBuffers(const std::vector<float>& vertices);

  /**
   * Calculates Model View Projection Matrix to convert custom
   * 3d cartesian coordinates into screen coordinates
   */
  void CalculateMVPMatrix();

  /**
   * Returns the next color after the current color being used in the list of
   * colors to choose from for drawing/graphing
   * @return
   */
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

  FunctionHandler function_handler_; // Computes all vector calculus involved
  GraphHandler graph_handler_; // Handles 2D graph coordinate system (converting between coordinate types, drawing axes, etc)

  glm::vec2 mouse_pos_; // current mouse position
  bool left_down_; // true if the mouse is currently left clicked
  bool in_range_; // If the mouse is clicking and and in range of the particle shop

  ci::gl::GlslProgRef glsl_prog_; // stock shader
  ci::gl::BatchRef arrow_batch_; // Batch that holds all the arrows
  ci::gl::VertBatchRef      vertBatch; // holds vertices used with arrow_batch

  CurveHandler curve_handler_; // Handles functionality related to drawing curves (using pen or 2d graphing)
  bool pen_mode_ = false; // Whether the use is pen mode (drawing mode)

  double total_work_; // amount of work across a curve
  double divergence_; // divergence of the field
  double curl_; // 2d curl of the field
  bool conservative = false; // whether the field is conservative or not

  std::string equation_;  // Equation that the user inputs for graph 1
  std::string equation2_; // Equation that the user inputs for graph 2

  //3D stuff
  Shader shader_ = Shader("visualizer/shader.vs", "visualizer/shader.fs"); // Custom shader wrapper
  unsigned int vbo_, vao_; // IDs used to tell openGL to graph the vertices in the buffer
  glm::mat4 model, view, projection; // matrices used to transform 3d coordinate system into a 2d world

  //Axis stuff
  std::vector<float> axisvertices = { // vertices of the axes
      0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 0.0f, // first 3 coordinates are position attribute, second 3 is the color
      0.0f, -5.0f, 0.0f, 1.0f, 1.0f, 0.0f,
      5.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
      -5.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 5.0f, 1.0f, 1.0f, 0.0f,
      0.0f, 0.0f, -5.0f, 1.0f, 1.0f, 0.0f
  };

  Camera camera = Camera(glm::vec3(0, 0, 13.0f)); // Used to change user perspective (initially start in the positive z direction)
  bool first_mouse_ = true; // Determines whether it is the first time user is clicking in the 3d world

  glm::vec2 mousePos = glm::vec2(0, 0); // initial mouse position when first clicking in 3d world

  bool toggle3d = false; // Whether or not user is in 3d mode

  size_t vertex_amnt_; // amount of vertices that openGL needs to create lines between

  ci::Color arrow_color_ = ci::Color(0, 1, 6); // Color of arrows
  ci::Color axes_color_ = ci::Color(1, 1, 0); // Color of axes and calculations in top right (gold color)

};

} // namespace visualizer
} // namespace vectorfield

#endif  // VECTOR_FIELD_GRAPHER_VECTOR_FIELD_SIMULATOR_H
