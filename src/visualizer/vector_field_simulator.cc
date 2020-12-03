#include <visualizer/vector_field_simulator.h>
#define RADPERDEG 0.0174533

namespace vectorfield {

namespace visualizer {

FieldSimulator::FieldSimulator()
    : curve_handler_(ci::Color::white()),
      graph_handler_(kWindowSize)  {
  ci::app::setWindowSize(kWindowSize, kWindowSize);
}

void FieldSimulator::setup() {

  SetupTweakBar();

  graph_handler_.CreateCoordinateSystem();

  InitializeBatch();

  // Default value is the origin if Add particle is clicked
  // but no x position or y position is specified
  std::ostringstream strx, stry;
  strx << graph_handler_.GetOrigin().x;
  stry << graph_handler_.GetOrigin().y;
  x_pos_ = strx.str();
  y_pos_ = stry.str();

  InitializeBuffers(axisvertices);
}

void FieldSimulator::draw() {
  ci::gl::clear(ci::Color(ci::Color::black()), true);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mParams->draw();
  std::string field_equation =
      "F = (" + i_component_ + ")i + (" + j_component_ + ")j + ("+k_component_ +")k";
  ci::gl::drawString(
      field_equation,  // Draw User Inputted Functions
      vec2(kWindowSize / 2 - 5 * field_equation.length() / 2, 10),
      ci::Color(1, 1, 0));
  DrawFPS();
  DrawMouseCoordinates();

  if(!toggle3d) {
    DrawCalculations();
    particle_manager_.DrawParticles();
    curve_handler_.Render();
    ci::gl::color(0, 191, 255);
    arrow_batch_->draw();

    if(conservative) {
      ci::gl::drawString("Conservative: true", vec2(10, 85), ci::Color(1, 1, 0));
    } else {
      ci::gl::drawString("Conservative: false", vec2(10, 85),ci::Color(1, 1, 0));
    }

    ci::gl::color(1, 1, 0);

    //DrawGraphAxes();
    graph_handler_.DrawGraphAxes();

    if (left_down_ && in_range_) {
      particle_manager_.DrawMouseParticle(mouse_pos_);
    }
  } else {

    CalculateMVPMatrix();

    glBindVertexArray(VAO2);
    glDrawArrays(GL_LINES, 0, line_amnt); // arrow vertices + axis vertices
    glBindVertexArray(0); // unbind
  }
}

void FieldSimulator::update() {
  particle_manager_.UpdateParticles(i_component_, j_component_);
}

void FieldSimulator::button(size_t id) {
  if(id == 0) { // Draw Field Button
    if(toggle3d) {
      Initialize3DArrowVertices();
    } else {
      vertBatch->clear();
      vertBatch = ci::gl::VertBatch::create(GL_LINES);
      InitializeFieldVectors();
      arrow_batch_ = ci::gl::Batch::create(*vertBatch, mGlslProg2);
      conservative = function_handler_.IsConservative(
          i_component_, j_component_, kVectorScale, 1000);
    }
  } else if(id == 1) { // Clear Field Button
    if(toggle3d) {
      InitializeBuffers(axisvertices);
      line_amnt = 18;
    } else {
      ClearArrows();
    }
  } else if(id == 2) { // Add Particle Button
    if(!toggle3d) {
      particle_manager_.AddParticle(
          5, glm::vec2(std::stod(x_pos_), std::stod(y_pos_)));
    }
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
    if(!toggle3d) {
      curve_handler_.CreateGraph();
      curve_handler_.CalculateGraphCoordinates(kScale, equation_);
      if (!equation2_.empty()) {
        curve_handler_.CreateGraph();
        curve_handler_.CalculateGraphCoordinates(kScale, equation2_);
      }
    }
  } else if(id == 10) { // Clear Graph button
    curve_handler_.ClearGraphs();
  } else if(id == 11) {
    curve_handler_.UndoGraph();
  } else if(id == 12) {
    toggle3d = !toggle3d;
    if(toggle3d) {
      InitializeBuffers(axisvertices);
      line_amnt = 18;
    }
  }
}

ci::Color FieldSimulator::GetNextColor() {
  color_index_ = (color_index_ + 1) % kPenColors.size();
  return kPenColors[color_index_];
}

void FieldSimulator::mouseDrag(ci::app::MouseEvent event) {
  if(!toggle3d) {
    mouse_pos_ = event.getPos();
    vec2 cart_mouse_coords = graph_handler_.ConvertScreenToCartesian(mouse_pos_);

    divergence_ = function_handler_.EvaluateDivergence(
        i_component_, j_component_, cart_mouse_coords.x, cart_mouse_coords.y);
    curl_ = function_handler_.Evaluate2DCurl(
        i_component_, j_component_, cart_mouse_coords.x, cart_mouse_coords.y);

    if(event.isLeftDown()) {
      mouse_pos_ = event.getPos();
      left_down_ = true; // used to check whether a particle is being held
    }
  }

  if(pen_mode_) {
    curve_handler_.ApplyStroke(event.getPos());
  }

  if(toggle3d) {
    if (firstMouse) {
      mousePos = event.getPos();
      firstMouse = false;
    }

    float xoffset = event.getX() - mousePos.x;
    float yoffset = mousePos.y - event.getY();
    mousePos = event.getPos();
    camera.ProcessMouseMovement(xoffset, yoffset);
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

  particle_manager_.AddReturnParticle(event.getPos());
}

void FieldSimulator::mouseWheel(ci::app::MouseEvent event) {
  camera.ProcessMouseScroll(event.getWheelIncrement());
}

void FieldSimulator::keyDown(ci::app::KeyEvent event) {
  switch (event.getCode()) {
    case ci::app::KeyEvent::KEY_SPACE: {
      Initialize3DArrowVertices();
      break;
    }
  }
}

void FieldSimulator::ClearArrows() {
  vertBatch->clear();
  arrow_batch_ = ci::gl::Batch::create(*vertBatch, mGlslProg2);
}

void FieldSimulator::Initialize3DArrowVertices() {
  std::vector<float> dynamic_vec;

  for(int x=0; x < 18; x++) {
    dynamic_vec.push_back(axisvertices[x]);
  }

  for (int x = -4; x < 5; x++) {
    for (int y = -4; y < 5; y++) {
      for (int z = -4; z < 5; z++) {
        dynamic_vec.push_back(static_cast<float>(x));
        dynamic_vec.push_back(static_cast<float>(z));
        dynamic_vec.push_back(static_cast<float>(y)); // y - axis is the upward direction, so to make it the cartesian coordinate system switch y and z
        vec3 vel = function_handler_.Evaluate3DFunction(i_component_, j_component_, k_component_, x, y, z);
        glm::vec3 scaled = image_scaling_factor_ * vel;
        dynamic_vec.push_back(static_cast<float>(x) + scaled.x);
        dynamic_vec.push_back(static_cast<float>(z) + scaled.z);
        dynamic_vec.push_back(static_cast<float>(y) + scaled.y);
      }
    }
  }

  line_amnt = dynamic_vec.size();
  InitializeBuffers(dynamic_vec);
}

void FieldSimulator::CalculateMVPMatrix() {
  ourShader.use();

  //Reset matrices
  model = glm::mat4(1.0f);
  view = glm::mat4(1.0f);
  projection = glm::mat4(1.0f);

  projection = glm::perspective(glm::radians(camera.Zoom), ci::app::getWindowAspectRatio(), 0.1f, 100.0f); // create 3d perspective
  view = camera.GetViewMatrix(); // Change to camera space

  // rotate camera view
  const float radius = 10.0f;
  float camX = sin(0.5*ci::app::getElapsedSeconds()) * radius;
  float camZ = cos(0.5* ci::app::getElapsedSeconds()) * radius;
  view = view * glm::lookAt(glm::vec3(camX, 0.9, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

  ci::mat4 mvp = projection * view * model;

  ourShader.setMat4("mvp", mvp); //Send mvp matrix to shader
}

void FieldSimulator::InitializeArrowVertices(int x, int y, const glm::vec2& velocity) {
  ci::gl::color( 255, 255, 255 );
  //glm::vec2 start(graph_handler_.GetOrigin().x + graph_handler_.GetXUnit() * x, graph_handler_.GetOrigin().y - graph_handler_.GetYUnit() * y);
  vec2 start(graph_handler_.ConvertCartesianToScreen(vec2(x, y)));

  glm::vec2 end(graph_handler_.GetOrigin().x + graph_handler_.GetXUnit() * x + graph_handler_.GetXUnit() * image_scaling_factor_ * velocity.x,
                graph_handler_.GetOrigin().y - graph_handler_.GetYUnit() * y - graph_handler_.GetYUnit()* image_scaling_factor_ * velocity.y);
  //vec2 end(graph_handler_.ConvertCartesianToScreen(vec2(start.x + image_scaling_factor_ * velocity.x, start.y + image_scaling_factor_ * velocity.y)));

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
}

void FieldSimulator::DrawMouseCoordinates() {
  std::ostringstream strx, stry;
  strx << mouse_pos_.x;
  stry << mouse_pos_.y;
  ci::gl::drawString("Mouse Position: (" + strx.str() + ", " + stry.str() + ")",
                     vec2(10, 25), ci::Color(1, 1, 0));
}

void FieldSimulator::DrawCalculations() {
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
}

void FieldSimulator::DrawFPS() {
  // Render FPS
  std::stringstream strFps;
  strFps << int( getAverageFps() ) << " FPS";
  ci::gl::drawString( strFps.str(), vec2( 10, 10 ), ci::Color( 1, 1, 0 ) );
}

void FieldSimulator::InitializeFieldVectors() {
  for(int x=-kVectorScale+1; x <= kVectorScale-1; x++) {
    for(int y=-kVectorScale+1; y <= kVectorScale-1; y++) {
      double valx = (double) x;
      double valy = (double) y;
      auto velocity = function_handler_.EvaluateFunction(i_component_, j_component_, valx, valy);
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
  mParams->addParam( "Function k-Comp", &k_component_ );
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

  mParams->addSeparator("3D");
  mParams->addButton("Toggle 3D", [ & ]() { button( 12); });
}

void FieldSimulator::InitializeBatch() {
  //Used to draw all the arrows with on draw call
  graph_handler_.InitializeTikMarkBatch();

  vertBatch = ci::gl::VertBatch::create(GL_LINES);
  mGlslProg2 = ci::gl::getStockShader( ci::gl::ShaderDef().color() );
  arrow_batch_ = ci::gl::Batch::create(*vertBatch, mGlslProg2);
}

void FieldSimulator::InitializeBuffers(const std::vector<float>& vertices) {
  //3D GRAPH SETUP

  glEnable(GL_DEPTH_TEST);

  glGenVertexArrays(1, &VAO2);
  glGenBuffers(1, &VBO2);
  glBindVertexArray(VAO2);

  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

  //position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  /*
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);*/

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
} // namespace visualizer
} // namespace vectorfield
