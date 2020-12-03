#ifndef VECTOR_FIELD_GRAPHER_GRAPH_HANDLER_H
#define VECTOR_FIELD_GRAPHER_GRAPH_HANDLER_H

#include "cinder/gl/gl.h"

namespace vectorfield {

using glm::vec2;

class GraphHandler {
 public:
  GraphHandler(size_t window_size);
  void DrawGraphAxes();
  void CreateCoordinateSystem();
  void SetupTikMarkVertices();
  vec2 ConvertScreenToCartesian(const vec2& screen_coords);
  vec2 ConvertCartesianToScreen(const vec2& cart_coords);
  void InitializeTikMarkBatch();
  vec2 GetOrigin() const;
  double GetXUnit() const;
  double GetYUnit() const;

 private:
  void SetupAxisArrowVerts(const glm::vec2& start, const glm::vec2& end,
                     float arrow_base, float arrow_height);

  const int kScale = 10;
  const size_t kGraphMargin = 50; // Dist from sides of window that graph axes are drawn
  const float kArrowHeight = 3.0f; // Height of arrowhead
  const float kArrowBase = 10.0f; // Distance that the base of the arrow spans

  glm::vec2 origin_; // Window coordinates of position (0, 0) of the graph axes
  double x_unit_; // Amount of pixels that a single x unit is
  double y_unit_; // Amount of pixels that a single y unit is
  size_t window_size_;

  ci::gl::VertBatchRef tik_vert_batch;
  ci::gl::GlslProgRef       mGlslProg;
  ci::gl::BatchRef          mBatch;
};
}

#endif  // VECTOR_FIELD_GRAPHER_GRAPH_HANDLER_H
