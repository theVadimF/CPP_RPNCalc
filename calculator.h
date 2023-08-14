#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stack>
#include <string>

#include "calc_stack.h"

namespace rpn {

class Calculator : calc_stack {
 public:
  Calculator() {}
  explicit Calculator(const std::string &input);
  void load_stack(const std::string &input);
  double calculate(double x);

 private:
  void process_node(std::stack<calc_node> &st, double x);
  double process_operator(type_t type, double a, double b);
  double process_function(type_t type, double a);
};

}  // namespace rpn

#endif  // CALCULATOR_H
