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
  forces_.clear();
}

void CurveHandler::CalculateCurveForces(const string& i_comp, const string& j_comp) {
  if(!strokes_.empty()) {
    for(const std::vector<glm::vec2> &stroke : strokes_) {
      for(size_t point_index = 0; point_index < stroke.size(); point_index++) {
        double x_val = (stroke[point_index].x - 350)/30;
        double y_val = -1*(stroke[point_index].y - 350)/30; //Dist from origin then divide by the unit scale

        if(abs(x_val) < 10 && abs(y_val) < 10) {
          vec2 force = function_handler_.EvaluateFunction(i_comp, j_comp, x_val, y_val);
          forces_.push_back(force);
        }
      }
    }
  }
}

void CurveHandler::CalculateWork() {

  std::cout << "CALCULATING WORK....." << std::endl;
  double total_work = 0;
  if(!strokes_.empty()) {
    for(const std::vector<glm::vec2> &stroke : strokes_) {
      for(size_t point_index = 0; point_index < stroke.size()-1; point_index++) {
        double x_val = (stroke[point_index].x - 350)/30;
        double y_val = -1*(stroke[point_index].y - 350)/30; //Dist from origin then divide by the unit scale
        double x_val2 = (stroke[point_index + 1].x - 350)/30;
        double y_val2 = -1*(stroke[point_index + 1].y - 350)/30; //Dist from origin then divide by the unit scale
        vec2 delta_disp = vec2(x_val2-x_val, y_val2 - y_val);
        double work_increment = glm::dot(forces_[point_index], delta_disp);
        std::cout << "DELTA FORCE: " << forces_[point_index] << "DELTA DISP: " << delta_disp << "WORK: " << work_increment << std::endl;
        total_work+=work_increment;
      }

      double endx = (stroke.back().x - 350) /30;
      double endy = (350 - stroke.back().y) / 30;
      double begx = (stroke.front().x - 350) / 30;
      double begy = (350 - stroke.front().y)/30;
      double endval = .5 * endx * endx + .5 * endy * endy;
      double begval = .5 * begx * begx + .5 * begy * begy;
      double diff = endval - begval;
      std::cout << "WORK BY FTLT: " << endval << " " << begval << " " << diff << std::endl;
    }
  }


  std::cout << "TOTAL WORK: " << total_work << std::endl;

  /*
  if(!strokes_.empty()) {
    for(const std::vector<glm::vec2> &stroke : strokes_) {
      for(size_t point_index = 0; point_index < stroke.size()-1; point_index++) {
        double x_val = (stroke[point_index].x - 350)/30;
        double y_val = -1*(stroke[point_index].y - 350)/30; //Dist from origin then divide by the unit scale
        double x_val2 = (stroke[point_index + 1].x - 350)/30;
        double y_val2 = -1*(stroke[point_index + 1].y - 350)/30; //Dist from origin then divide by the unit scale
        double slope = (y_val2 - y_val)/(x_val2 - x_val);
        std::ostringstream str_slope, strx, stry;
        str_slope << slope;
        strx << x_val;
        stry << y_val;

        //std::cout << "SLOPE: " << str_slope.str() << std::endl;
        std::string expression = "y = " + str_slope.str() + "* (x - "+strx.str()+") + "+stry.str();
        std::cout << expression << std::endl;

        vec2 delta_force = forces_[point_index+1] - forces_[point_index];
        vec2 delta_disp = vec2(x_val2-x_val, y_val2 - y_val);
        double work_increment = glm::dot(delta_force, delta_disp);
        std::cout << "DELTA FORCE: " << delta_force << "DELTA DISP: " << delta_disp << "WORK: " << work_increment << std::endl;
        total_work+=work_increment;
      }
    }
  }*/
}
/*
void CurveHandler::CalculateGraphPoints(const std::string &equation) {
  CreateStroke();
  for(size_t x = 0; x < 700; x++) {
    for(size_t y = 0; y < 700; y++) {
      bool solution = function_handler.SolveEquation(x, y, equation);
      if(solution) {
        ApplyStroke(glm::vec2(x, y));
      }
    }
  }
}*/
} // namespace vectorfield
