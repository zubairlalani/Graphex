#ifndef VECTOR_FIELD_GRAPHER_CURVE_HANDLER_H
#define VECTOR_FIELD_GRAPHER_CURVE_HANDLER_H

#include "cinder/gl/gl.h"
#include "cinder/Color.h"
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

 private:
  std::vector<std::vector<glm::vec2>> strokes_;
  ci::Color stroke_color_;
};
} // namespace vectorfield

#endif  // VECTOR_FIELD_GRAPHER_CURVE_HANDLER_H
