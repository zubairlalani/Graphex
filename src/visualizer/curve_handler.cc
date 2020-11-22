#include "visualizer/curve_handler.h"

namespace vectorfield {

CurveHandler::CurveHandler(const ci::Color &stroke_color)
    : stroke_color_(stroke_color){}

void CurveHandler::Render() const {
  ci::gl::color(stroke_color_);

  if(!strokes_.empty()) {
    for(const std::vector<glm::vec2> &stroke : strokes_) {
      ci::gl::begin(GL_LINE_STRIP);
      ci::gl::lineWidth(5.0);
      for(const glm::vec2 &point : stroke) {
        ci::gl::vertex(point);
      }
      ci::gl::end();
    }
  }
}

void CurveHandler::CreateStroke() {
  strokes_.emplace_back();
}

void CurveHandler::ApplyStroke(const glm::vec2 &brush_screen_coords) {
  strokes_.back().push_back(brush_screen_coords);
}

void CurveHandler::ChangeColor(const ci::Color& color) {
  stroke_color_ = color;
}

void CurveHandler::Clear() {
  strokes_.clear();
}
} // namespace vectorfield
