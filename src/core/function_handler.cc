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

  return vec2(i_expr_.value(), -1*j_expr_.value()); // y-coordinate of cinder is opposite of graph y direction
  //field_vectors_[{x, y}] = glm::vec2(i_expr_.value(), j_expr_.value());
}
/*
void FunctionHandler::SetFieldComponents(const string& i_comp, const string& j_comp) {
  //TODO: Make a Field class instead later
  i_component_ = i_comp;
  j_component_ = j_comp;
}

string FunctionHandler::GetIComponent() const {
  return i_component_;
}

string FunctionHandler::GetJComponent() const {
  return j_component_;
}*/

}
