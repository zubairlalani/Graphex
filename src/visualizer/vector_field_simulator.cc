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

  //3D GRAPH SETUP

  glEnable(GL_DEPTH_TEST);

  glGenVertexArrays(1, &VAO2);
  glGenBuffers(1, &VBO2);

  glBindVertexArray(VAO2);

  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(axisvertices), axisvertices, GL_STATIC_DRAW);

  //position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  /*
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);*/

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void FieldSimulator::draw() {

  float currentFrame = ci::app::getElapsedSeconds();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  ci::gl::clear(ci::Color(ci::Color::black()), true);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //----------------------------------------------------------------------

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
    curve_handler_.Render();
    ci::gl::color(0, 191, 255);
    mBatch2->draw();

    if(conservative) {
      ci::gl::drawString("Conservative: true", vec2(10, 85), ci::Color(1, 1, 0));
    } else {
      ci::gl::drawString("Conservative: false", vec2(10, 85),ci::Color(1, 1, 0));
    }

    ci::gl::color(1, 1, 0);

    DrawGraphAxes();
    ci::gl::color(1, 1, 0);
    mBatch->draw();

    if (left_down_ && in_range_) {
      particle_manager_.DrawMouseParticle(mouse_pos_);
    }
  } else {
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

    //Send matrices to Shader
    ourShader.setMat4("model", model);
    ourShader.setMat4("view", view);
    ourShader.setMat4("projection", projection);

    glBindVertexArray(VAO2);
    glDrawArrays(GL_LINES, 0, line_amnt); // arrow vertices + axis vertices

    glBindVertexArray(0);
  }
}

void FieldSimulator::update() {
  particle_manager_.UpdateParticles(i_component_, j_component_);
}

void FieldSimulator::button(size_t id) {
  if(id == 0) { // Draw Field Button
    if(!toggle3d) {
      vertBatch->clear();
      vertBatch = ci::gl::VertBatch::create(GL_LINES);
      InitializeFieldVectors();
      mBatch2 = ci::gl::Batch::create(*vertBatch, mGlslProg2);
      conservative = function_handler_.IsConservative(i_component_, j_component_, kVectorScale, 1000);
    } else {
      //Initialize3DFieldVectors();
    }
  } else if(id == 1 && !toggle3d) { // Clear Field Button
    ClearArrows();
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
      glEnable(GL_DEPTH_TEST);

      glGenVertexArrays(1, &VAO2);
      glGenBuffers(1, &VBO2);

      glBindVertexArray(VAO2);

      glBindBuffer(GL_ARRAY_BUFFER, VBO2);
      glBufferData(GL_ARRAY_BUFFER, sizeof(axisvertices), axisvertices,
                   GL_STATIC_DRAW);

      // position attribute
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                            (void*)0);
      glEnableVertexAttribArray(0);
      /*
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3 * sizeof(float)));
      glEnableVertexAttribArray(1);*/

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);

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
    double x_val = (mouse_pos_.x - origin_.x) / x_unit_;
    double y_val = (origin_.y - mouse_pos_.y) / y_unit_;
    divergence_ = function_handler_.EvaluateDivergence(
        i_component_, j_component_, x_val, y_val);
    curl_ = function_handler_.Evaluate2DCurl(i_component_, j_component_, x_val,
                                             y_val);
  }

  if(pen_mode_) {
    curve_handler_.ApplyStroke(event.getPos());
  }

  if(event.isLeftDown()) {
    mouse_pos_ = event.getPos();
    left_down_ = true;
  }


  //asdfasdf
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
      //float dynamic_arr[10 * 10 * 10+1];
      std::vector<float> dynamic_vec;
      //memcpy(&dynamic_vec[0], axisvertices, 66*sizeof(float));
      for(int x=0; x < 18; x++) {
        dynamic_vec.push_back(axisvertices[x]);
      }

      for (int x = -4; x < 5; x++) {
        for (int y = -4; y < 5; y++) {
          for (int z = -4; z < 5; z++) {
            dynamic_vec.push_back(static_cast<float>(x));
            dynamic_vec.push_back(static_cast<float>(z));
            dynamic_vec.push_back(static_cast<float>(y)); // y - axis is the upward direction, so to make it the cartesian coordinate system switch y and z
            /*
            dynamic_vec.push_back(1.0f);
            dynamic_vec.push_back(1.0f);
            dynamic_vec.push_back(0.0f);*/
            //glm::vec3 vel(x, y, z);
            vec3 vel = function_handler_.Evaluate3DFunction(i_component_, j_component_, k_component_, x, y, z);
            glm::vec3 scaled = image_scaling_factor_ * vel;
            dynamic_vec.push_back(static_cast<float>(x) + scaled.x);
            dynamic_vec.push_back(static_cast<float>(z) + scaled.z);
            dynamic_vec.push_back(static_cast<float>(y) + scaled.y);
            /*
            dynamic_vec.push_back(1.0f);
            dynamic_vec.push_back(1.0f);
            dynamic_vec.push_back(0.0f);*/
          }
        }
      }

      float dynamic_arr[dynamic_vec.size()];
      line_amnt = dynamic_vec.size();
      std::copy(dynamic_vec.begin(), dynamic_vec.end(), dynamic_arr);
      for(float i: dynamic_arr) {
        std::cout << i << std::endl;
      }

      glGenVertexArrays(2, &VAO2);// Axis VAO & VBO
      glGenBuffers(2, &VBO2);

      glBindVertexArray(VAO2);

      glBindBuffer(GL_ARRAY_BUFFER, VBO2);
      glBufferData(GL_ARRAY_BUFFER, sizeof(dynamic_arr), dynamic_arr, GL_STATIC_DRAW);

      //position attribute
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
      glEnableVertexAttribArray(0);
      /*
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3 * sizeof(float)));
      glEnableVertexAttribArray(1);*/

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);

      break;
    }

    case ci::app::KeyEvent::KEY_RIGHT:
      camera.ProcessKeyboard(RIGHT, deltaTime);
      break;

    case ci::app::KeyEvent::KEY_LEFT:
      camera.ProcessKeyboard(LEFT, deltaTime);
      break;

    case ci::app::KeyEvent::KEY_UP:
      camera.ProcessKeyboard(FORWARD, deltaTime);
      break;

    case ci::app::KeyEvent::KEY_DOWN:
      camera.ProcessKeyboard(BACKWARD, deltaTime);
      break;
  }
}

void FieldSimulator::ClearArrows() {
  vertBatch->clear();
  mBatch2 = ci::gl::Batch::create(*vertBatch, mGlslProg2);
  //field_vectors_.clear();
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

  if(!toggle3d) {
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
      //field_vectors_[{x, y}] = velocity;
      InitializeArrowVertices(x, y, velocity);
    }
  }
}

void FieldSimulator::Initialize3DFieldVectors() {
  std::vector<float> dynamic_vec;
  //memcpy(&dynamic_vec[0], axisvertices, 66*sizeof(float));
  for(int x=0; x < 36; x++) {
    dynamic_vec.push_back(axisvertices[x]);
  }

  for (int x = -4; x < 5; x++) {
    for (int y = -4; y < 5; y++) {
      for (int z = -4; z < 5; z++) {
        dynamic_vec.push_back(static_cast<float>(x));
        dynamic_vec.push_back(static_cast<float>(y));
        dynamic_vec.push_back(static_cast<float>(z));

        glm::vec3 vel(x, y, z);
        glm::vec3 scaled = 0.5f * vel;
        dynamic_vec.push_back(static_cast<float>(x) + scaled.x);
        dynamic_vec.push_back(static_cast<float>(y) + scaled.y);
        dynamic_vec.push_back(static_cast<float>(z) + scaled.z);
      }
    }
  }

  line_amnt = dynamic_vec.size();

  float dynamic_arr[dynamic_vec.size()];
  std::copy(dynamic_vec.begin(), dynamic_vec.end(), dynamic_arr);
  for(float i: dynamic_arr) {
    std::cout << i << std::endl;
  }

  glGenVertexArrays(2, &VAO2);// Axis VAO & VBO
  glGenBuffers(2, &VBO2);

  glBindVertexArray(VAO2);

  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(dynamic_arr), dynamic_arr, GL_STATIC_DRAW);

  //position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
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
