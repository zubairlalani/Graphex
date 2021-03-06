#include "visualizer/curve_handler.h"

namespace vectorfield {

CurveHandler::CurveHandler(const ci::Color &stroke_color)
    : stroke_color_(stroke_color){}

void CurveHandler::Render() const {
  ci::gl::color(stroke_color_);

  //Connects all the points within each stroke
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

  //Connects all points contained by each curve
  if(!graphs.empty()) {
    for(const auto & graph : graphs) {
      ci::gl::begin(GL_LINE_STRIP);
      ci::gl::lineWidth(10.0);
      for(const glm::vec2 &point : graph) {
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
  //Calculate the force along each stroke point
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

double CurveHandler::CalculateWork() {

  std::cout << "CALCULATING WORK....." << std::endl;

  //Sums all the little work increments between each consecutive points
  double total_work = 0;
  if(!strokes_.empty()) {
    for(const std::vector<glm::vec2> &stroke : strokes_) {
      for (size_t point_index = 0; point_index < stroke.size() - 1;
           point_index++) {
        double x_val = (stroke[point_index].x - 350) / 30;
        double y_val = -1 * (stroke[point_index].y - 350) /
                       30;  // Dist from origin then divide by the unit scale
        double x_val2 = (stroke[point_index + 1].x - 350) / 30;
        double y_val2 = -1 * (stroke[point_index + 1].y - 350) /
                        30;  // Dist from origin then divide by the unit scale
        vec2 delta_disp = vec2(x_val2 - x_val, y_val2 - y_val);
        double work_increment = glm::dot(forces_[point_index], delta_disp);
        total_work += work_increment;
      }
    }
  }

  return total_work;
}

void CurveHandler::CalculateGraphCoordinates(int graph_scale, const string& equation) {
  //For every a bunch of x values, calculates the y value and then saves those coordinates
  double stepSize = 0.01;
  for (double x = -graph_scale; x < graph_scale+1+stepSize/2; x+= stepSize)
  {
    double y = function_handler_.SolveEquation(x, equation);
    if(abs(y) <= static_cast<double>(graph_scale)) {
      double x_screen_coord = 30*x + 350; // convert from graph coordinates to screen coordinates
      double y_screen_coord = 350 - 30*y;
      graphs.back().emplace_back(vec2(x_screen_coord, y_screen_coord));
    }
  }
}

void CurveHandler::CreateGraph() {
  graphs.emplace_back();
}

void CurveHandler::ClearGraphs(){
  graphs.clear();
}

void CurveHandler::UndoGraph() {
  if(graphs.size() > 0) {
    graphs.pop_back();
  }
}
} // namespace vectorfield
