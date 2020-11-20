#ifndef VECTOR_FIELD_GRAPHER_FUNCTION_HANDLER_H
#define VECTOR_FIELD_GRAPHER_FUNCTION_HANDLER_H

#include <exprtk.hpp>
#include <glm/glm.hpp>

namespace vectorfield {

using glm::vec2;
using std::string;

class FunctionHandler {
 public:
  vec2 EvaluateFunction(const string& i_comp, const string& j_comp, double x_val, double y_val);


 private:

  exprtk::symbol_table<double> symbol_table_; // Used for Math expression parsing --> holds diff types of variable, constants, etc
  exprtk::expression<double> i_expr_; // Expression that user provided
  exprtk::expression<double> j_expr_; // Expression that user provided
  exprtk::parser<double> parser_; // Parses string version of expression into exprtk library math expression

};

}

#endif  // VECTOR_FIELD_GRAPHER_FUNCTION_HANDLER_H
