#ifndef VECTOR_FIELD_GRAPHER_FUNCTION_HANDLER_H
#define VECTOR_FIELD_GRAPHER_FUNCTION_HANDLER_H

#include <exprtk.hpp>
#include "cinder/gl/gl.h"

namespace vectorfield {

using glm::vec2;
using std::string;

class FunctionHandler {
 public:
  vec2 EvaluateFunction(const string& i_comp, const string& j_comp, double x_val, double y_val);

  bool SolveEquation(double x, double y, const std::string& equation);

  double EvaluateDivergence(const string& i_comp, const string& j_comp, double x_val, double y_val);

  double Evaluate2DCurl(const string& i_comp, const string& j_comp, double x_val, double y_val);

 private:

  exprtk::symbol_table<double> symbol_table_; // Used for Math expression parsing --> holds diff types of variable, constants, etc
  exprtk::expression<double> i_expr_; // Expression that user provided
  exprtk::expression<double> j_expr_; // Expression that user provided
  exprtk::parser<double> parser_; // Parses string version of expression into exprtk library math expression
  exprtk::function_compositor<double> compositor_;
  exprtk::function_compositor<double>::function function;
  exprtk::expression<double> equation_expr_;

  exprtk::expression<double> i_expr_2; // Expression that user provided
  exprtk::expression<double> j_expr_2; // Expression that user provided
  exprtk::symbol_table<double> symbol_table2_;
  exprtk::parser<double> parser2_;

  exprtk::expression<double> i_expr_3; // Expression that user provided
  exprtk::expression<double> j_expr_3; // Expression that user provided
  exprtk::symbol_table<double> symbol_table3_;
  exprtk::parser<double> parser3_;


};

}

#endif  // VECTOR_FIELD_GRAPHER_FUNCTION_HANDLER_H
