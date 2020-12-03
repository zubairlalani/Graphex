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

vec3 FunctionHandler::Evaluate3DFunction(const string& i_comp, const string& j_comp, const string& k_comp,
                                         double x_val, double y_val, double z_val) {
  exprtk::symbol_table<double> symbol_table;
  symbol_table.add_variable("x", x_val);
  symbol_table.add_variable("y", y_val);
  symbol_table.add_variable("z", z_val);
  symbol_table.add_constants();

  exprtk::expression<double> i_expr;
  exprtk::expression<double> j_expr;
  exprtk::expression<double> k_expr;


  i_expr.register_symbol_table(symbol_table);
  j_expr.register_symbol_table(symbol_table);
  k_expr.register_symbol_table(symbol_table);

  parser_.compile(i_comp, i_expr);
  parser_.compile(j_comp, j_expr);
  parser_.compile(k_comp, k_expr);

  return vec3(i_expr.value(), j_expr.value(), k_expr.value());
}

double FunctionHandler::SolveEquation(double x, const std::string& equation) {
  exprtk::symbol_table<double> table;
  exprtk::expression<double> equation_expr_;

  table.add_variable("x", x);
  table.add_constants();

  equation_expr_.register_symbol_table(table);
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
