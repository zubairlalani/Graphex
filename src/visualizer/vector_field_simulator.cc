#include <visualizer/vector_field_simulator.h>
#define RADPERDEG 0.0174533

namespace vectorfield {

namespace visualizer {

FieldSimulator::FieldSimulator() : curve_handler_(ci::Color::white()) {
  ci::app::setWindowSize(kWindowSize, kWindowSize);
}

void FieldSimulator::setup() {

  SetupTweakBar();

  CreateCoordinateSystem();

  InitializeBatch();

  // Default value is the origin if Add particle is clicked
  // but no x position or y position is specified
  std::ostringstream strx, stry;
  strx << origin_.x;
  stry << origin_.y;
  x_pos_ = strx.str();
  y_pos_ = stry.str();
}

void FieldSimulator::draw() {

  ci::gl::clear(ci::Color(ci::Color::black()), true);

  //if(pen_mode_) {
    curve_handler_.Render();
  //}

  ci::gl::color( 0, 191, 255 );
  mParams->draw();
  mBatch2->draw();

  DrawFPS();
  DrawMouseCoordinates();
  ci::gl::color( 1, 1, 0 );
  DrawGraphAxes();
  ci::gl::color( 1, 1, 0 );
  mBatch->draw();

  std::string field_equation = "F = ("+i_component_+")i + ("+j_component_+")j";
  ci::gl::drawString(field_equation, //Draw User Inputted Functions
                     vec2(kWindowSize/2 - 5*field_equation.length()/2, 10),
                     ci::Color( 1, 1, 0 ));

  if(left_down_ && in_range_) {
    particle_manager_.DrawMouseParticle(mouse_pos_);
  }
}

void FieldSimulator::update() {
  particle_manager_.UpdateParticles(i_component_, j_component_);
}

void FieldSimulator::button(size_t id) {
  if(id == 0) { // Draw Field Button
    vertBatch->clear();
    vertBatch = ci::gl::VertBatch::create(GL_LINES);
    InitializeFieldVectors();
    mBatch2 = ci::gl::Batch::create(*vertBatch, mGlslProg2);
  } else if(id == 1) { // Clear Field Button
    ClearArrows();
  } else if(id == 2) { // Add Particle Button
    particle_manager_.AddParticle(5,
                                  glm::vec2(std::stod(x_pos_),std::stod(y_pos_)));
  } else if(id == 3) { // Clear Particles Button
    particle_manager_.ClearParticles();
  } else if(id == 4) { // Enter Pen Mode
    pen_mode_ = true;
  } else if(id == 5) { // Exit Pen Mode
    pen_mode_ = false;
    curve_handler_.Clear();
  } else if(id == 6) { // Erase
    curve_handler_.Clear();
  } else if(id == 7) { // Change Pen Color
    curve_handler_.ChangeColor(GetNextColor());
  } else if(id == 8) { // Calculate Work
    curve_handler_.CalculateCurveForces(i_component_, j_component_);
    total_work_ = curve_handler_.CalculateWork();
  } else if(id == 9) { // Graph button
    curve_handler_.CreateGraph();
    curve_handler_.CalculateGraphCoordinates(kScale, equation_);
    if(!equation2_.empty()) {
      curve_handler_.CreateGraph();
      curve_handler_.CalculateGraphCoordinates(kScale, equation2_);
    }
  } else if(id == 10) { // Clear Graph button
    curve_handler_.ClearGraphs();
  } else if(id == 11) {
    curve_handler_.UndoGraph();
  }
}

ci::Color FieldSimulator::GetNextColor() {
  color_index_ = (color_index_ + 1) % kPenColors.size();
  return kPenColors[color_index_];
}

void FieldSimulator::mouseDrag(ci::app::MouseEvent event) {
  mouse_pos_ = event.getPos();
  double x_val = (mouse_pos_.x - origin_.x)/x_unit_;
  double y_val = (origin_.y - mouse_pos_.y)/y_unit_;
  divergence_ = function_handler_.EvaluateDivergence(i_component_, j_component_, x_val, y_val);
  curl_ = function_handler_.Evaluate2DCurl(i_component_, j_component_,x_val, y_val);

  if(pen_mode_) {
    curve_handler_.ApplyStroke(event.getPos());
  }

  if(event.isLeftDown()) {
    mouse_pos_ = event.getPos();
    left_down_ = true;
  }
}

void FieldSimulator::mouseUp(ci::app::MouseEvent event) {
  if(left_down_ && in_range_) {
    particle_manager_.AddParticle(5, event.getPos());
  }
  left_down_ = false;
  in_range_ = false;
}

void FieldSimulator::mouseDown(ci::app::MouseEvent event) {
  if(pen_mode_) {
    curve_handler_.CreateStroke();
  }
  vec2 dist = vec2(event.getPos().x - particle_manager_.GetParticleShopPos().x,
                   event.getPos().y - particle_manager_.GetParticleShopPos().y);
  double length = glm::length(dist);
  if(length < 10) {
    in_range_ = true;
  }
}

void FieldSimulator::ClearArrows() {
  vertBatch->clear();
  mBatch2 = ci::gl::Batch::create(*vertBatch, mGlslProg2);
  field_vectors_.clear();
}

void FieldSimulator::InitializeArrowVertices(int x, int y, const glm::vec2& velocity) {
  ci::gl::color( 255, 255, 255 );
  glm::vec2 start(origin_.x + x_unit_ * x, origin_.y - y_unit_ * y);
  glm::vec2 end(origin_.x + x_unit_ * x + x_unit_ * image_scaling_factor_ * velocity.x,
                origin_.y - y_unit_ * y - y_unit_* image_scaling_factor_ * velocity.y);
  vec2 direction_vec(end.x - start.x, end.y - start.y);

  //Make the direction vector a unit vector
  direction_vec /= sqrt((direction_vec.x * direction_vec.x)
                        + (direction_vec.y * direction_vec.y));

  //Create a vector that is the direction vector rotated 90 degrees to form a basis
  vec2 basis_vec(direction_vec.y, -1* direction_vec.x);

  vec2 arrow_point_one = end - (kArrowHeight * direction_vec) - (-1*kArrowBase * basis_vec / 2.0f);
  vec2 arrow_point_two = end - (kArrowHeight * direction_vec) - (kArrowBase * basis_vec / 2.0f);

  vertBatch->vertex(start);
  vertBatch->vertex(end);
  vertBatch->vertex(end);
  vertBatch->vertex(arrow_point_one);
  vertBatch->vertex(end);
  vertBatch->vertex(arrow_point_two);

  DrawTikMarks();
}

void FieldSimulator::DrawMouseCoordinates() {
  std::ostringstream strx, stry;
  strx << mouse_pos_.x;
  stry << mouse_pos_.y;
  ci::gl::drawString("Mouse Position: (" + strx.str() + ", " + stry.str() + ")",
                     vec2(10, 25), ci::Color(1, 1, 0));

  std::ostringstream divstr;

  divstr << divergence_;

  ci::gl::drawString("Divergence: " + divstr.str(),
                     vec2(10, 40), ci::Color(1, 1, 0));


  std::ostringstream curlstr;
  curlstr << curl_;
  ci::gl::drawString("Curl: < 0, 0, " + curlstr.str() + ">",
                     vec2(10, 55), ci::Color(1, 1, 0));

  std::ostringstream workstr;

  workstr << total_work_;

  ci::gl::drawString("Approx Work: " + workstr.str(),
                     vec2(10, 70), ci::Color(1, 1, 0));

  particle_manager_.DrawParticles();
}

void FieldSimulator::DrawFPS() {
  // Render FPS
  std::stringstream strFps;
  strFps << int( getAverageFps() ) << " FPS";
  ci::gl::drawString( strFps.str(), vec2( 10, 10 ), ci::Color( 1, 1, 0 ) );
}

void FieldSimulator::DrawGraphAxes() {
  ci::gl::drawLine(vec2(kWindowSize/2, kGraphMargin),
                   vec2(kWindowSize/2, kWindowSize - kGraphMargin ));
  ci::gl::drawLine(vec2(kGraphMargin, (kWindowSize)/2), //-kInputBoxHeight
                   vec2(kWindowSize-kGraphMargin, (kWindowSize)/2));
  DrawTikMarks();
}

void FieldSimulator::InitializeFieldVectors() {
  for(int x=-kVectorScale; x <= kVectorScale; x++) {
    for(int y=-kVectorScale; y <= kVectorScale; y++) {
      double valx = (double) x;
      double valy = (double) y;
      auto velocity = function_handler_.EvaluateFunction(i_component_, j_component_, valx, valy);
      field_vectors_[{x, y}] = velocity;
      InitializeArrowVertices(x, y, velocity);
    }
  }
}

void FieldSimulator::SetupTweakBar() {
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
  mParams->addParam( "Arrow Scale", &image_scaling_factor_ );

  mParams->addSeparator("Particle Params");
  mParams->addButton("Add Particle", [ & ]() { button( 2 ); });
  mParams->addParam("X Position", &x_pos_);
  mParams->addParam("Y Position", &y_pos_);
  mParams->addButton("Clear Particles", [ & ]() { button( 3 ); });

  mParams->addSeparator("Canvas");
  mParams->addButton("Enter Pen Mode", [ & ]() { button( 4 ); });
  mParams->addButton("Exit Pen Mode", [ & ]() { button( 5 ); });
  mParams->addButton("Erase", [ & ]() { button( 6 ); });
  mParams->addButton("Change Pen Color", [ & ]() { button( 7 ); });
  mParams->addButton("Calculate Work", [ & ]() { button( 8 ); });

  mParams->addSeparator("Grapher");
  mParams->addParam("y = ", &equation_);
  mParams->addParam("y2 = ", &equation2_);
  mParams->addButton("Graph", [ & ]() { button( 9); });
  mParams->addButton("Clear Graph", [ & ]() { button( 10); });
  mParams->addButton("Undo", [ & ]() { button( 11); });
  //mParams->addParam("Enter Equation", &equation);
}

void FieldSimulator::CreateCoordinateSystem() {
  origin_ = glm::vec2(kWindowSize/2, (kWindowSize)/2); // origin of graph in screen coordinates
  x_unit_ = static_cast<double>(kWindowSize - 2*kGraphMargin)/(2*kScale); // Calculate how many screen pixels a single x unit is
  y_unit_ = x_unit_;
}

void FieldSimulator::DrawTikMarks() {
  size_t length = 5;
  tik_vert_batch = ci::gl::VertBatch::create(GL_LINES);
  for(int i=-kScale+1; i<kScale; i++) {
    tik_vert_batch->vertex(vec2(origin_.x + i * x_unit_, origin_.y + length));
    tik_vert_batch->vertex(vec2(origin_.x + i * x_unit_, origin_.y - length));
    tik_vert_batch->vertex(vec2(origin_.x + length, origin_.y + i * y_unit_));
    tik_vert_batch->vertex(vec2(origin_.x - length, origin_.y + i * y_unit_));
  }



  vec2 end(kWindowSize - kGraphMargin, origin_.y);
  vec2 start(kWindowSize - kGraphMargin - 10, origin_.y);
  DrawArrow(start , end, 2*kArrowBase, kArrowHeight+10);

  end = vec2(kGraphMargin, origin_.y);
  start = vec2(kGraphMargin + 10, origin_.y);
  DrawArrow(start , end, 2*kArrowBase, kArrowHeight+10);

  end = vec2(origin_.x, kWindowSize-kGraphMargin);
  start = vec2(origin_.x, kWindowSize-kGraphMargin-10);
  DrawArrow(start , end, 2*kArrowBase, kArrowHeight+10);

  end = vec2(origin_.x, kGraphMargin);
  start = vec2(origin_.x, kGraphMargin+10);
  DrawArrow(start , end, 2*kArrowBase, kArrowHeight+10);

  mBatch = ci::gl::Batch::create(*tik_vert_batch, mGlslProg);
}

void FieldSimulator::DrawArrow(const glm::vec2& start, const glm::vec2& end,
                               float arrow_base, float arrow_height) {
  vec2 direction_vec(end.x - start.x, end.y - start.y);

  //Make the direction vector a unit vector
  direction_vec /= sqrt((direction_vec.x * direction_vec.x)
                        + (direction_vec.y * direction_vec.y));

  //Create a vector that is the direction vector rotated 90 degrees to form a basis
  vec2 basis_vec(direction_vec.y, -1* direction_vec.x);
  vec2 arrow_point_one = end - ((arrow_height) * direction_vec) - (-1*(arrow_base) * basis_vec / 2.0f);
  vec2 arrow_point_two = end - ((arrow_height) * direction_vec) - ((arrow_base) * basis_vec / 2.0f);

  tik_vert_batch->vertex(end);
  tik_vert_batch->vertex(arrow_point_one);
  tik_vert_batch->vertex(end);
  tik_vert_batch->vertex(arrow_point_two);
}

void FieldSimulator::InitializeBatch() {
  //Used to draw all the arrows with on draw call
  tik_vert_batch = ci::gl::VertBatch::create(GL_LINES);
  mGlslProg = ci::gl::getStockShader( ci::gl::ShaderDef().color() );
  mBatch = ci::gl::Batch::create(*tik_vert_batch, mGlslProg);

  vertBatch = ci::gl::VertBatch::create(GL_LINES);
  mGlslProg2 = ci::gl::getStockShader( ci::gl::ShaderDef().color() );
  mBatch2 = ci::gl::Batch::create(*vertBatch, mGlslProg2);
}
} // namespace visualizer
} // namespace vectorfield
