#ifndef VECTOR_FIELD_GRAPHER_CURVE_HANDLER_H
#define VECTOR_FIELD_GRAPHER_CURVE_HANDLER_H

#include <core/function_handler.h>

#include "cinder/Color.h"
#include "cinder/gl/gl.h"
namespace vectorfield {
//Uses Patrick's code from cinder demo
class CurveHandler {
 public:
  CurveHandler(const ci::Color& stroke_color);
  void Render() const;
  void CreateStroke();
  void ApplyStroke(const glm::vec2& brush_screen_coords);
  void ChangeColor(const ci::Color& color);
  void Clear();
  double CalculateWork();
  void CalculateCurveForces(const string& i_comp, const string& j_comp);

 private:
  std::vector<std::vector<glm::vec2>> strokes_;
  ci::Color stroke_color_;
  FunctionHandler function_handler_;
  std::vector<glm::vec2> forces_;
};
} // namespace vectorfield

#endif  // VECTOR_FIELD_GRAPHER_CURVE_HANDLER_H
