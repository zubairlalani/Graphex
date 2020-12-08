#ifndef VECTOR_FIELD_GRAPHER_FUNCTION_HANDLER_H
#define VECTOR_FIELD_GRAPHER_FUNCTION_HANDLER_H

#include <exprtk.hpp>
#include "cinder/gl/gl.h"

namespace vectorfield {

using glm::vec2;
using glm::vec3;
using std::string;

/**
 * Contains functions that can use exprtk library in order evaluate different mathematical properties
 */
class FunctionHandler {
 public:
  /**
   * Given a 2d vector field function, evaluate the velocity at a given point
   * @param i_comp - i-hat component of the vector field function
   * @param j_comp - j-hat component of the vector field function
   * @param x_val - x_val in cartesian coordinates that the function should be evaluated at
   * @param y_val - y_val in cartesian coordinates that the function should be evaluated at
   * @return velocity at the given point on the field
   */
  vec2 EvaluateFunction(const string& i_comp, const string& j_comp, double x_val, double y_val);

  /**
   * Given a 3d vector field function, evaluate the velocity at a given 3d point
   * @param i_comp - i-hat component of the vector field function
   * @param j_comp - j-hat component of the vector field function
   * @param k_comp - k-hat component of the vector field function
   * @param x_val - x_val in cartesian coordinates that the function should be evaluated at
   * @param y_val - y_val in cartesian coordinates that the function should be evaluated at
   * @param z_val - z_val in cartesian coordinates that the function should be evaluated at
   * @return velocity at the given point on the 3d field
   */
  vec3 Evaluate3DFunction(const string& i_comp, const string& j_comp, const string& k_comp, double x_val, double y_val, double z_val);

  /**
   * Note: this function is used in order to graph 2d functions
   * Given a single variable equation, "y = ____", evaluate the equation at a given x_val
   * @param x - value for x that should be plugged into the equation
   * @param equation - user-inputted equation
   * @return value of y based on plugging in x into the equation
   */
  double SolveEquation(double x, const std::string& equation);

  /**
   * Calculates the divergence of a vector field at a given point
   * @param i_comp - i-hat component of the vector field function
   * @param j_comp - j-hat component of the vector field function
   * @param x_val - x value in cartesian that the mouse is at
   * @param y_val - y value in cartesian that the mouse is at
   * @return value of divergence at the point
   */
  double EvaluateDivergence(const string& i_comp, const string& j_comp, double x_val, double y_val);

  /**
   * Calculates the 2D curl of a vector field at a given point
   * @param i_comp - i-hat component of the vector field function
   * @param j_comp - j-hat component of the vector field function
   * @param x_val - x value in cartesian that the mouse is at
   * @param y_val - y value in cartesian that the mouse is at
   * @return value of the 2D curl, which is the k-hat component of the curl vector
   */
  double Evaluate2DCurl(const string& i_comp, const string& j_comp, double x_val, double y_val);

  /**
   * Determines whether a 2D vector field is conservative or not
   * @param i_comp - i-hat component of the vector field function
   * @param j_comp - j-hat component of the vector field function
   * @param scale - how many units the graph goes up to
   * @param accuracy_lvl - how accurate the prediction should be (the higher the number given, the more accurate)
   * @return true - if the field is conservative
   */
  bool IsConservative(const string& i_comp, const string& j_comp, int scale, size_t accuracy_lvl);


 private:

  exprtk::parser<double> parser_; // Parses string version of expression into exprtk library math expression

};

}

#endif  // VECTOR_FIELD_GRAPHER_FUNCTION_HANDLER_H
