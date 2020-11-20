#include <visualizer/vector_field_simulator.h>
#define RADPERDEG 0.0174533

namespace vectorfield {

namespace visualizer {

FieldSimulator::FieldSimulator() {
  ci::app::setWindowSize(kWindowSize, kWindowSize);
}

void FieldSimulator::setup() {

  mParams = ci::params::InterfaceGl::create(
      getWindow(), "App parameters",
      getWindow()->toPixels( ci::ivec2( 300, 200 ) )
  );

  mParams->addSeparator("Field Params");
  mParams->setPosition(glm::vec2(50, 500));
  mParams->addButton("Draw", [ & ]() { button( 0 ); });
  mParams->addButton("Clear", [ & ]() { button( 1 ); });
  mParams->addParam( "Function i-Comp", &i_component_ );
  mParams->addParam( "Function j-Comp", &j_component_ );

  mParams->addSeparator("Particle Params");
  mParams->addButton("Add Particle", [ & ]() { button( 2 ); });
  mParams->addButton("Clear Particles", [ & ]() { button( 3 ); });

  mParams->addSeparator("Arrow Params");
  mParams->addParam( "Arrow Scale", &image_scaling_factor_ );

  origin_ = glm::vec2(kWindowSize/2, (kWindowSize)/2);
  std::cout << "ORIGIN: " << origin_ << std::endl;

  x_unit_ = static_cast<double>(kWindowSize - 2*kGraphMargin)/(2*kScale);
  y_unit_ = x_unit_;
  std::cout << x_unit_ << std::endl;

  /*
  Experimenting with integration/derivatives:

  const std::string integrate_expr = "x^3";
  double x;
  table.add_variable("x", x);
  i_expr_.register_symbol_table(table);
  parser.compile(integrate_expr, i_expr_);
  double area = exprtk::integrate(i_expr_, x, -1.0, 1.0);
  x = 12.0;
  double derivation = exprtk::derivative(i_expr_, x);
  std::cout << "AREA: " << area << std::endl;
  std::cout << "SLOPE: " << derivation << std::endl;*/

}

void FieldSimulator::draw() {

  ci::gl::clear(ci::Color(0, 0, 0), true);

  mParams->draw();
  ci::gl::color(255, 255, 255);

  ci::gl::drawLine(vec2(kWindowSize/2, kGraphMargin),
                   vec2(kWindowSize/2, kWindowSize - kGraphMargin ));
  ci::gl::drawLine(vec2(kGraphMargin, (kWindowSize)/2), //-kInputBoxHeight
                   vec2(kWindowSize-kGraphMargin, (kWindowSize)/2));

  ci::gl::drawString("F = ("+i_component_+")i + ("+j_component_+")j", vec2(kWindowSize/2 - 30, 25));
  auto it = field_vectors_.begin();

  for (auto const& element : field_vectors_) {

    //TODO: Fix performance issues with drawing particle while drawing Field
    glm::vec2 start(origin_.x + x_unit_ *element.first.first, origin_.y - y_unit_ * element.first.second);
    glm::vec2 end(origin_.x + x_unit_ *element.first.first + x_unit_ * image_scaling_factor_ * element.second.x,
                  origin_.y - y_unit_ *element.first.second - y_unit_* image_scaling_factor_ * element.second.y);


    vec2 direction_vec(end.x - start.x, end.y - start.y);

    //Make the direction vector a unit vector
    direction_vec /= sqrt((direction_vec.x * direction_vec.x)
                          + (direction_vec.y * direction_vec.y));

    //Create a vector that is the direction vector rotated 90 degrees to form a basis
    vec2 basis_vec(direction_vec.y, -1* direction_vec.x);

    vec2 arrow_point_one = end - (kArrowHeight * direction_vec) - (-1*kArrowBase * basis_vec / 2.0f);
    vec2 arrow_point_two = end - (kArrowHeight * direction_vec) - (kArrowBase * basis_vec / 2.0f);

    ci::gl::drawLine(start, end);
    ci::gl::drawLine(end, arrow_point_one);
    ci::gl::drawLine(end, arrow_point_two);

  }

  particle_manager_.DrawParticles();

}

void FieldSimulator::update() {
  particle_manager_.UpdateParticles(i_component_, j_component_);
}

void FieldSimulator::button(size_t id) {
  if(id == 0) {

    for(int x=-kVectorScale; x <= kVectorScale; x++) {
      for(int y=-kVectorScale; y <= kVectorScale; y++) {
          double valx = (double) x;
          double valy = (double) y;

          field_vectors_[{x, y}] = function_handler_.EvaluateFunction(
              i_component_, j_component_, valx, valy);
      }
    }

  } else if(id == 1) {
    field_vectors_.clear();
  } else if(id == 2) {
    particle_manager_.AddParticle(5);
  } else if(id == 3) {
    particle_manager_.ClearParticles();
  }
}
} // namespace visualizer
} // namespace vectorfield
