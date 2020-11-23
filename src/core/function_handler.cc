#include "core/function_handler.h"

namespace vectorfield {
vec2 FunctionHandler::EvaluateFunction(const string& i_component, const string& j_component,
                                       double x_val, double y_val) {

  symbol_table_.add_variable("x", x_val);
  symbol_table_.add_variable("y", y_val);
  symbol_table_.add_constants();

  i_expr_.register_symbol_table(symbol_table_);
  j_expr_.register_symbol_table(symbol_table_);

  parser_.compile(i_component, i_expr_);
  parser_.compile(j_component, j_expr_);

  return vec2(i_expr_.value(), j_expr_.value());
}

bool FunctionHandler::SolveEquation(double x, double y, const std::string& equation) {
  symbol_table_.add_variable("x", x);
  symbol_table_.add_variable("y", y);
  symbol_table_.add_constants();

  equation_expr_.register_symbol_table(symbol_table_);
  parser_.compile(equation, equation_expr_);

  return equation_expr_.value();
}
}
