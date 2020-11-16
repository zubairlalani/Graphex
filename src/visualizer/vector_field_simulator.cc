#include <visualizer/vector_field_simulator.h>
#define RADPERDEG 0.0174533

namespace vectorfield {

namespace visualizer {

FieldSimulator::FieldSimulator() {
  ci::app::setWindowSize(kWindowSize, kWindowSize);
}

void FieldSimulator::setup() {
  //mPosition = getWindowCenter();
  //gui->addSlider("Position", &mPosition, glm::vec2(0,0), getWindowSize());
  auto img = loadImage( loadAsset( "vector.png" ) );
  auto im2 = loadAsset("vector.png");
  texture = ci::gl::Texture2d::create(img);
  //mUi = SuperCanvas::create("hello");
  //mUi->addSpacer();

  mParams = ci::params::InterfaceGl::create(
      getWindow(), "App parameters",
      getWindow()->toPixels( ci::ivec2( 400, 120 ) )
  );
  mParams->setPosition(glm::vec2(50, 500));

  mParams->addButton("Save Function", [ & ]() { button( 0 ); });
  /*
  mParams->addButton("Medium Particle", [ & ]() { button( 1 ); });
  mParams->addButton("Large Particle", [ & ]() { button( 2 ); });
  mParams->addButton("Increase Speed", [ & ]() { button( 3 ); });
  mParams->addButton("Decrease Speed", [ & ]() { button( 4 ); });
  mParams->addButton("Clear", [ & ]() { button( 5 ); });*/

  mParams->addParam( "Function i-Comp", &i_component_ );
  mParams->addParam( "Function j-Comp", &j_component_ );

}

void FieldSimulator::draw() {

  ci::gl::clear(ci::Color(0, 100, 100), true);

  //ci::gl::draw(texture);

  mParams->draw();


  float arrow_size = 30.0f;
  glm::vec3 start(kWindowSize/2, kWindowSize/2, 0);
  glm::vec3 end(kWindowSize/2 + arrow_size, kWindowSize/2, 0);
  ci::gl::drawVector(start, end, 12.0f, 6.0f);
  //gui->draw();

  ci::gl::drawLine(glm::vec2(kWindowSize/2, kGraphMargin),
                   glm::vec2(kWindowSize/2, kWindowSize - kGraphMargin - kInputBoxHeight));
  ci::gl::drawLine(glm::vec2(kGraphMargin, (kWindowSize-kInputBoxHeight)/2),
                   glm::vec2(kWindowSize-kGraphMargin, (kWindowSize-kInputBoxHeight)/2));




  //ci::gl::drawSolidRect(ci::Rectf(glm::vec2(60, 60), glm::vec2(70, 100)));
  //ci::gl::drawSolidTriangle(glm::vec2(50, 100), glm::vec2(80, 100), glm::vec2(65, 110));

  /*
  ci::gl::pushModelView();
  ci::gl::translate(60, 60);
  //ci::gl::scale(glm::vec2(2, 2));
  ci::gl::rotate(glm::radians(30.0f));
  ci::gl::translate(-60, -60);
  ci::gl::drawSolidRect(ci::Rectf(glm::vec2(60, 60), glm::vec2(70, 100)));
  ci::gl::drawSolidTriangle(glm::vec2(50, 100), glm::vec2(80, 100), glm::vec2(65, 110));
  ci::gl::popModelView();*/
}

void FieldSimulator::update() {

}

void FieldSimulator::button(size_t id) {
  if(id == 0) {

    double x = 1;
    double y = 5;
    table.add_variable("x", x);
    table.add_variable("y", y);
    table.add_constants();

    i_expr_.register_symbol_table(table);
    j_expr_.register_symbol_table(table);

    parser.compile(i_component_, i_expr_);
    parser.compile(j_component_, j_expr_);

    std::cout << "PRINTING CALCULATION: " << std::endl;
    std::cout << i_component_ << std::endl;
    std::cout << j_component_ << std::endl;
    std::cout << i_expr_.value() << std::endl;
    std::cout << j_expr_.value() << std::endl;
    std::cout << "" << std::endl;
    /*
    int x;
    int y;

    table.add_constants();
    for(x=-kScale; x< kScale; x++) {
      for(y=-kScale; y < kScale; y++) {
        table.add_variable("x", x);
        table.add_variable("y", y);
        i_expr_.register_symbol_table(table);
        j_expr_.register_symbol_table(table);
        parser.compile(i_component_, i_expr_);
        parser.compile(j_component_, j_expr_);

        table.clear();

        //vectors_[{x, y}] = glm::vec2(i_expr_, j_expr_);

        std::cout << "PRINTING CALCULATION: " << std::endl;
        std::cout << i_component_ << std::endl;
        std::cout << j_component_ << std::endl;
        std::cout << i_expr_.value() << std::endl;
        std::cout << j_expr_.value() << std::endl;
        std::cout << "" << std::endl;
      }
    } */

  }
}
} // namespace visualizer
} // namespace vectorfield
