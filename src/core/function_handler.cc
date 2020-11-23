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

double FunctionHandler::EvaluateDivergence(const string& i_comp, const string& j_comp,
                                         double x_val, double y_val) {


  symbol_table2_.add_variable("x", x_val);
  symbol_table2_.add_variable("y", y_val);
  symbol_table2_.add_constants();

  i_expr_2.register_symbol_table(symbol_table2_);
  j_expr_2.register_symbol_table(symbol_table2_);

  parser2_.compile(i_comp, i_expr_2);
  parser2_.compile(j_comp, j_expr_2);

  double dPdx = exprtk::derivative(i_expr_2, "x");
  double dQdy = exprtk::derivative(j_expr_2, "y");
  return dPdx + dQdy;
}

double FunctionHandler::Evaluate2DCurl(const string& i_comp, const string& j_comp,
                                       double x_val, double y_val) {

  symbol_table3_.add_variable("x", x_val);
  symbol_table3_.add_variable("y", y_val);
  symbol_table3_.add_constants();

  i_expr_3.register_symbol_table(symbol_table3_);
  j_expr_3.register_symbol_table(symbol_table3_);

  parser2_.compile(i_comp, i_expr_3);
  parser2_.compile(j_comp, j_expr_3);

  double dPdy = exprtk::derivative(i_expr_3, "y");
  double dQdx = exprtk::derivative(j_expr_3, "x");
  std::cout << dPdy << " | " << dQdx << std::endl;
  return dQdx - dPdy;
}
}
