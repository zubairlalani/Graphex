#include "visualizer/graph_handler.h"
namespace vectorfield {

GraphHandler::GraphHandler(size_t window_size) : window_size_(window_size) {}

void GraphHandler::InitializeTikMarkBatch() {
  tik_vert_batch_ = ci::gl::VertBatch::create(GL_LINES);
  stock_shader_ = ci::gl::getStockShader( ci::gl::ShaderDef().color() );
  batch_ = ci::gl::Batch::create(*tik_vert_batch_, stock_shader_);
}

void GraphHandler::CreateCoordinateSystem() {
  origin_ = glm::vec2(window_size_/2, (window_size_)/2); // origin of graph in screen coordinates
  x_unit_ = static_cast<double>(window_size_ - 2*kGraphMargin)/(2*kScale); // Calculate how many screen pixels a single x unit is
  y_unit_ = x_unit_;
}

void GraphHandler::DrawGraphAxes() {
  ci::gl::color(1, 1, 0);

  //Draw axes
  ci::gl::drawLine(vec2(window_size_/2, kGraphMargin),
                   vec2(window_size_/2, window_size_ - kGraphMargin ));
  ci::gl::drawLine(vec2(kGraphMargin, (window_size_)/2), //-kInputBoxHeight
                   vec2(window_size_-kGraphMargin, (window_size_)/2));
  //Draw tik marks
  SetupTikMarkVertices();
  batch_->draw();
}

void GraphHandler::SetupTikMarkVertices() {
  size_t length = 5;
  tik_vert_batch_ = ci::gl::VertBatch::create(GL_LINES);
  for(int i=-kScale+1; i<kScale; i++) {
    tik_vert_batch_->vertex(vec2(origin_.x + i * x_unit_, origin_.y + length));
    tik_vert_batch_->vertex(vec2(origin_.x + i * x_unit_, origin_.y - length));
    tik_vert_batch_->vertex(vec2(origin_.x + length, origin_.y + i * y_unit_));
    tik_vert_batch_->vertex(vec2(origin_.x - length, origin_.y + i * y_unit_));
  }

  //Sets up each of the 4 arrows of at the end of the axes

  vec2 end(window_size_ - kGraphMargin, origin_.y);
  vec2 start(window_size_ - kGraphMargin - 10, origin_.y);
  SetupAxisArrowVerts(start, end, 2 * kArrowBase, kArrowHeight + 10);

  end = vec2(kGraphMargin, origin_.y);
  start = vec2(kGraphMargin + 10, origin_.y);
  SetupAxisArrowVerts(start, end, 2 * kArrowBase, kArrowHeight + 10);

  end = vec2(origin_.x, window_size_-kGraphMargin);
  start = vec2(origin_.x, window_size_-kGraphMargin-10);
  SetupAxisArrowVerts(start, end, 2 * kArrowBase, kArrowHeight + 10);

  end = vec2(origin_.x, kGraphMargin);
  start = vec2(origin_.x, kGraphMargin+10);
  SetupAxisArrowVerts(start, end, 2 * kArrowBase, kArrowHeight + 10);

  batch_ = ci::gl::Batch::create(*tik_vert_batch_, stock_shader_);
}

void GraphHandler::SetupAxisArrowVerts(const glm::vec2& start, const glm::vec2& end,
                                 float arrow_base, float arrow_height) {
  vec2 direction_vec(end.x - start.x, end.y - start.y);

  //Make the direction vector a unit vector
  direction_vec /= sqrt((direction_vec.x * direction_vec.x)
                        + (direction_vec.y * direction_vec.y));

  //Create a vector that is the direction vector rotated 90 degrees to form a basis
  vec2 basis_vec(direction_vec.y, -1* direction_vec.x);
  vec2 arrow_point_one = end - ((arrow_height) * direction_vec) - (-1*(arrow_base) * basis_vec / 2.0f);
  vec2 arrow_point_two = end - ((arrow_height) * direction_vec) - ((arrow_base) * basis_vec / 2.0f);

  tik_vert_batch_->vertex(end);
  tik_vert_batch_->vertex(arrow_point_one);
  tik_vert_batch_->vertex(end);
  tik_vert_batch_->vertex(arrow_point_two);
}

vec2 GraphHandler::GetOrigin() const {
  return origin_;
}

double GraphHandler::GetXUnit() const {
  return x_unit_;
}

double GraphHandler::GetYUnit() const {
  return y_unit_;
}

vec2 GraphHandler::ConvertScreenToCartesian(const vec2& screen_coords) {
  double x_val = (screen_coords.x - origin_.x) / x_unit_;
  double y_val = (origin_.y - screen_coords.y) / y_unit_;
  return vec2(x_val, y_val);
}

vec2 GraphHandler::ConvertCartesianToScreen(const vec2& cart_coords) {
  float x_val = origin_.x + x_unit_ * cart_coords.x;
  float y_val = origin_.y - y_unit_ * cart_coords.y;
  return vec2(x_val, y_val);
}
}