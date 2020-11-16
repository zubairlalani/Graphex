#include <visualizer/vector_field_simulator.h>
#define RADPERDEG 0.0174533

namespace vectorfield {

namespace visualizer {

FieldSimulator::FieldSimulator() {
  ci::app::setWindowSize(kWindowSize, kWindowSize);
}

void FieldSimulator::setup() {
  //gui = pretzel::PretzelGui::create("Circle settings");
  //mPosition = getWindowCenter();
  //gui->addSlider("Position", &mPosition, glm::vec2(0,0), getWindowSize());
  //auto img = loadImage( loadAsset( "vector.png" ) );
  //texture = ci::gl::Texture2d::create(img);
  //mUi = SuperCanvas::create("hello");
  //mUi->addSpacer();
  /*
  mUi->addLabel( "WE CAN DO BETTER THAN PARAMS! ^_^", FontSize::SMALL );
  mUi->addLabel( "DOUBLE CLICK BASIC TO MINIMIZE", FontSize::SMALL );
  mUi->addLabel( "CLICK BASIC AND DRAG TO MOVE", FontSize::SMALL );
  mUi->addSpacer();

  mUi->addLabel( "BASIC SLIDER:", FontSize::SMALL );
  mUi->addSliderf( "RED", &mRed );

  mUi->addSpacer();
  mUi->addLabel( "SLIDER W/ OPTION: CROSSFADER", FontSize::SMALL );
  mUi->addSliderf( "GREEN", &mGreen, 0.0, 1.0, Sliderf::Format().crossFader() );

  mUi->addSpacer();
  mUi->addLabel( "SLIDER W/ OPTION: NO LABEL", FontSize::SMALL );
  mUi->addSliderf( "BLUE", &mBlue, 0.0, 1.0, Sliderf::Format().label( false ) );

  mUi->addMultiSlider( "BG COLOR", {
      MultiSlider::Data( "RED", &mRed ),
      MultiSlider::Data( "GREEN", &mGreen ),
      MultiSlider::Data( "BLUE", &mBlue )
  } );
  mUi->autoSizeToFitSubviews();*/

  //mUi->load( getSaveLoadPath() );

  ci::TextBox tbox = ci::TextBox().alignment(ci::TextBox::RIGHT).text("mStr").size(ci::ivec2(kWindowSize/2, kWindowSize/2));
  tbox.setColor(ci::ColorA(1,1,1,1));
  tbox.setBackgroundColor(ci::ColorA(0.0f, 0.0f, 0.0f, 0.0f));
  tbox.setPremultiplied(true);
  mTextTexture = ci::gl::Texture2d::create(tbox.render());
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
  mParams->addParam( "Function", &mFunction );


}

void FieldSimulator::draw() {

  ci::gl::clear(ci::Color(0, 100, 100), true);

  mParams->draw();
  /*
  ci::gl::ScopedBlend alphaBlend(true);
  ci::gl::ScopedBlendPremult preAlpha();
  ci::gl::ScopedColor color(ci::Color(1, 1, 1));
  ci::gl::draw(mTextTexture);
  */
  ci::gl::ScopedBlendPremult blend;
  ci::gl::draw( mTextTexture );

  //gui->draw();

  ci::gl::drawLine(glm::vec2(kWindowSize/2, kGraphMargin),
                   glm::vec2(kWindowSize/2, kWindowSize - kGraphMargin - kInputBoxHeight));
  ci::gl::drawLine(glm::vec2(kGraphMargin, (kWindowSize-kInputBoxHeight)/2),
                   glm::vec2(kWindowSize-kGraphMargin, (kWindowSize-kInputBoxHeight)/2));
  //ci::gl::draw(texture);
  ci::gl::drawSolidRect(ci::Rectf(glm::vec2(60, 60), glm::vec2(70, 100)));
  ci::gl::drawSolidTriangle(glm::vec2(50, 100), glm::vec2(80, 100), glm::vec2(65, 110));

  ci::gl::pushModelView();
  ci::gl::translate(60, 60);
  //ci::gl::scale(glm::vec2(2, 2));
  ci::gl::rotate(glm::radians(30.0f));
  ci::gl::translate(-60, -60);
  ci::gl::drawSolidRect(ci::Rectf(glm::vec2(60, 60), glm::vec2(70, 100)));
  ci::gl::drawSolidTriangle(glm::vec2(50, 100), glm::vec2(80, 100), glm::vec2(65, 110));
  ci::gl::popModelView();
}

void FieldSimulator::update() {

}

void FieldSimulator::button(size_t id) {

}
} // namespace visualizer
} // namespace vectorfield
