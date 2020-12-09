#include "core/function_handler.h"

namespace vectorfield {
vec2 FunctionHandler::EvaluateFunction(const string& i_component, const string& j_component,
                                       double x_val, double y_val) {
  //NOTE: symbol_table, i_expr, j_expr are not made member variables due to errors
  // with using the same symbol tables and expr variables for different evaluations.
  // May update this in the future

  exprtk::symbol_table<double> symbol_table; // Contains all symbols that will be used in the expression
  symbol_table.add_variable("x", x_val);
  symbol_table.add_variable("y", y_val);
  symbol_table.add_constants(); // adds stuff like pi, log, infinity, etc to the symbol table

  // string version of expression will be parsed into these vars
  exprtk::expression<double> i_expr;
  exprtk::expression<double> j_expr;

  i_expr.register_symbol_table(symbol_table);
  j_expr.register_symbol_table(symbol_table);

  // Parses string expressions in the exprtk vars, and evaluates them
  parser_.compile(i_component, i_expr);
  parser_.compile(j_component, j_expr);

  return vec2(i_expr.value(), j_expr.value());
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

  exprtk::symbol_table<double> symbol_table;
  symbol_table.add_variable("x", x_val);
  symbol_table.add_variable("y", y_val);
  symbol_table.add_constants();

  exprtk::expression<double> i_expr;
  exprtk::expression<double> j_expr;

  i_expr.register_symbol_table(symbol_table);
  j_expr.register_symbol_table(symbol_table);

  parser_.compile(i_comp, i_expr);
  parser_.compile(j_comp, j_expr);

  // takes the derivative of one var while treating the other as a constant,
  // so it is essentially a partial derivative
  double dPdx = exprtk::derivative(i_expr, "x");
  double dQdy = exprtk::derivative(j_expr, "y");
  return dPdx + dQdy;
}

double FunctionHandler::Evaluate2DCurl(const string& i_comp, const string& j_comp,
                                       double x_val, double y_val) {

  exprtk::symbol_table<double> symbol_table;
  symbol_table.add_variable("x", x_val);
  symbol_table.add_variable("y", y_val);
  symbol_table.add_constants();

  exprtk::expression<double> i_expr;
  exprtk::expression<double> j_expr;

  i_expr.register_symbol_table(symbol_table);
  j_expr.register_symbol_table(symbol_table);

  parser_.compile(i_comp, i_expr);
  parser_.compile(j_comp, j_expr);

  double dPdy = exprtk::derivative(i_expr, "y");
  double dQdx = exprtk::derivative(j_expr, "x");
  return dQdx - dPdy;
}

bool FunctionHandler::IsConservative(const string& i_comp, const string& j_comp, int scale, size_t accuracy_lvl) {
  // Checking for conservativity usually requires symbolic integration/differentiation
  // Avoided this by just checking the curl at many points
  // When in R2, curl F = 0 everywhere on the field if the field is conservative
  // So if at any point, the curl is not zero, then we know it is not conservative
  double epsilon = 0.001;
  srand(static_cast<unsigned>(time(0)));
  float random_x_val;
  float random_y_val;
  for(int i = 0; i < accuracy_lvl; i++) {
    random_x_val = -scale + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(scale - (-scale))));
    random_y_val = -scale + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(scale - (-scale))));
    double val = Evaluate2DCurl(i_comp, j_comp, random_x_val, random_y_val);
    if(abs(val) > epsilon){
      return false;
    }
  }
  return true;
}

} // namespace vectorfield
