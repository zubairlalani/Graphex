#ifndef VECTOR_FIELD_GRAPHER_GRAPH_HANDLER_H
#define VECTOR_FIELD_GRAPHER_GRAPH_HANDLER_H

#include "cinder/gl/gl.h"

namespace vectorfield {

using glm::vec2;

class GraphHandler {
 public:
  /**
   * Initializes graph_handler with the window size so that a coordinate system can be made
   * @param window_size - sidelength of the square window size
   */
  GraphHandler(size_t window_size);

  /**
   * Draws the graph axes with tik marks and axis arrows
   */
  void DrawGraphAxes();

  /**
   * Based on the window size, determines where the origin of the 2D graph will be
   * and how many screen coordinates is 1 unit
   */
  void CreateCoordinateSystem();

  /**
   * Calculates where each tik mark should be and adds it to a batch
   */
  void SetupTikMarkVertices();

  /**
   * Given screen coordinates converts them to a point on the graph
   * @param screen_coords - coordinates on the cinder window screen
   * @return vec2 that holds the cartesian coordinates
   */
  vec2 ConvertScreenToCartesian(const vec2& screen_coords);

  /**
   * Given cartesian coordinates (coordinates on the graph) converts
   * them to cinder screen coordinates
   * @param cart_coords
   * @return vec2 that holds the screen coordinates
   */
  vec2 ConvertCartesianToScreen(const vec2& cart_coords);

  /**
   * Initializes batch that holds the tik marks
   */
  void InitializeTikMarkBatch();

  /**
   * Gets the origin of the graph in screen coordinates
   * @return vec2 that represents position on screen of the origin
   */
  vec2 GetOrigin() const;

  /**
   * Get how many pixels the x unit is
   * @return double that represents the amount of screen units a single x unit is
   */
  double GetXUnit() const;

  /**
   * Get how many pixels the y unit is
   * @return double that represents the amount of screen units a single y unit is
   */
  double GetYUnit() const;

 private:

  /**
   * Adds vertices to batch that will draw an arrow from the given starting position
   * to the ending position with the given arrow base and arrow height
   * @param start - where the arrow starts
   * @param end - where the arrow ends
   * @param arrow_base - how wide the base of the arrow is
   * @param arrow_height - how long the head is
   */
  void SetupAxisArrowVerts(const glm::vec2& start, const glm::vec2& end,
                     float arrow_base, float arrow_height);

  const int kScale = 10; // How many units wide/high the graph is
  const size_t kGraphMargin = 50; // Dist from sides of window that graph axes are drawn
  const float kArrowHeight = 3.0f; // Height of arrowhead
  const float kArrowBase = 10.0f; // Distance that the base of the arrow spans

  glm::vec2 origin_; // Window coordinates of position (0, 0) of the graph axes
  double x_unit_; // Amount of pixels that a single x unit is
  double y_unit_; // Amount of pixels that a single y unit is
  size_t window_size_; // sidelength of the square window

  ci::gl::VertBatchRef
      tik_vert_batch_; // Holds vertices to draw all the tik marks
  ci::gl::GlslProgRef stock_shader_; // Stock shader
  ci::gl::BatchRef batch_; // Batch to draw axes
};
}

#endif  // VECTOR_FIELD_GRAPHER_GRAPH_HANDLER_H
