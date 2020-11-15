#include <visualizer/vector_field_simulator.h>
#define RADPERDEG 0.0174533

namespace vectorfield {

namespace visualizer {

FieldSimulator::FieldSimulator() {
  ci::app::setWindowSize(kWindowSize, kWindowSize);

}

void FieldSimulator::setup() {
  auto img = loadImage( loadAsset( "vector.png" ) );
  texture = ci::gl::Texture2d::create(img);
}

void FieldSimulator::draw() {
  ci::gl::drawSolidRect(ci::Rectf(glm::vec2(60, 60), glm::vec2(70, 100)));
  ci::gl::drawSolidTriangle(glm::vec2(50, 100), glm::vec2(80, 100), glm::vec2(65, 110));

  ci::gl::pushModelView();
  ci::gl::translate(60, 60);
  ci::gl::scale(glm::vec2(2, 2));
  ci::gl::rotate(glm::radians(30.0f));
  ci::gl::translate(-60, -60);
  ci::gl::drawSolidRect(ci::Rectf(glm::vec2(60, 60), glm::vec2(70, 100)));
  ci::gl::drawSolidTriangle(glm::vec2(50, 100), glm::vec2(80, 100), glm::vec2(65, 110));
  ci::gl::popModelView();
}

void FieldSimulator::update() {

}

} // namespace visualizer
} // namespace vectorfield
