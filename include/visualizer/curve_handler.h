#ifndef VECTOR_FIELD_GRAPHER_CURVE_HANDLER_H
#define VECTOR_FIELD_GRAPHER_CURVE_HANDLER_H

#include <core/function_handler.h>

#include "cinder/Color.h"
#include "cinder/gl/gl.h"
namespace vectorfield {

/**
 * Handles all functions related to drawing curves using the pen feature,
 * and graphing regular equations
 * Note: referenced Patrick's pictionary cinder demo
 */
class CurveHandler {
 public:

  /**
   * Initializes original color that the curve will be in
   * @param stroke_color - initial color of the curves
   */
  CurveHandler(const ci::Color& stroke_color);

  /**
   * Draws 2D graph/ pen drawing onto the window
   */
  void Render() const;

  /**
   * A new linestrip is being created
   */
  void CreateStroke();

  /**
   * Adds points to the last stroke created, as long as the mouse is still being clicked
   * @param brush_screen_coords - coordinates of the mouse where the curve will be drawn
   */
  void ApplyStroke(const glm::vec2& brush_screen_coords);

  /**
   * Switches pen color
   * @param color - color that curve should be drawn in
   */
  void ChangeColor(const ci::Color& color);

  /**
   * Clears all pen markings on the window
   */
  void Clear();

  /**
   * Calculates summation of the work across every stroke
   * @return double value that represents the work
   */
  double CalculateWork();

  /**
   * Calculates the force vector at every point along the pen markings
   * @param i_comp - i-hat component of the field
   * @param j_comp - j-hat component of the field
   */
  void CalculateCurveForces(const string& i_comp, const string& j_comp);

  /**
   * Solves the given equation for many points
   * @param graph_scale - how wide the graph is
   * @param equation - the equation that should be graphed
   */
  void CalculateGraphCoordinates(int graph_scale, const string& equation);

  /**
   * Create a new graph "stroke"
   */
  void CreateGraph();

  /**
   * Clears all the graphs on the screen
   */
  void ClearGraphs();

  /**
   * Removes the last graph drawn
   */
  void UndoGraph();

 private:
  // Everytime the mouse is clicked down and dragged a new stroke is created
  std::vector<std::vector<glm::vec2>> strokes_;  // List of all strokes with all the points that are contained by each stroke
  ci::Color stroke_color_; // Current color
  FunctionHandler function_handler_; // Used to evaluate work/solve equations for y coordinates
  std::vector<glm::vec2> forces_; // Force along each point within a stroke
  std::vector<std::vector<glm::vec2>> graphs; // similar to strokes_, contains all the points for each different graph curve

};
} // namespace vectorfield

#endif  // VECTOR_FIELD_GRAPHER_CURVE_HANDLER_H
