#ifndef VECTOR_FIELD_GRAPHER_FUNCTION_HANDLER_H
#define VECTOR_FIELD_GRAPHER_FUNCTION_HANDLER_H

#include <exprtk.hpp>
#include <glm/glm.hpp>

namespace vectorfield {

using glm::vec2;

class FunctionHandler {
 public:
  vec2 EvaluateFunction(const std::string& i_component, const std::string& j_component,
                        double x_val, double y_val);

 private:
  exprtk::symbol_table<double> symbol_table_; // Used for Math expression parsing --> holds diff types of variable, constants, etc
  exprtk::expression<double> i_expr_; // Expression that user provided
  exprtk::expression<double> j_expr_; // Expression that user provided
  exprtk::parser<double> parser_; // Parses string version of expression into exprtk library math expression

};

}

#endif  // VECTOR_FIELD_GRAPHER_FUNCTION_HANDLER_H
