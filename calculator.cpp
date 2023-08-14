#include "calculator.h"

#include <cmath>
#include <iostream>

namespace rpn {

Calculator::Calculator(const std::string &input) { load_stack(input); }

void Calculator::load_stack(const std::string &input) {
  calc_stack::load(input);
}

double Calculator::calculate(double x) {
  std::stack<calc_node> st_copy = calc_stack::st;
  std::stack<calc_node> tmp;
  while (!st_copy.empty()) {
    calc_stack::move_element(tmp, st_copy);
    // If node is not a number
    if (tmp.top().priority != 0) {
      process_node(tmp, x);
    }
  }
  double result = NAN;
  if (tmp.empty() || (tmp.top().type != number && tmp.top().type != is_x)) {
    throw std::runtime_error("Incorrect result type in calculate");
  }
  if (tmp.top().type == is_x) {
    result = x;
  } else {
    result = tmp.top().value;
  }
  return result;
}

void Calculator::process_node(std::stack<calc_node> &st, double x) {
  double result;

  type_t type = st.top().type;
  int priority = st.top().priority;
  st.pop();
  double b;
  if (st.top().type == is_x) {
    b = x;
  } else {
    b = st.top().value;
  }
  st.pop();
  // If node is an operator
  if (priority <= 2 || type == power) {
    double a;
    if (st.top().type == is_x) {
      a = x;
    } else {
      a = st.top().value;
    }
    st.pop();
    result = process_operator(type, a, b);
  } else {
    result = process_function(type, b);
  }
  st.push(calc_node(result, 0, number));
}

double Calculator::process_operator(type_t type, double a, double b) {
  double result = NAN;
  switch (type) {
    case plus:
      result = a + b;
      break;
    case minus:
      result = a - b;
      break;
    case multiply:
      result = a * b;
      break;
    case divide:
      if (b != 0) {
        result = a / b;
      }
      break;
    case power:
      result = pow(a, b);
      break;
    case modulus:
      result = fmod(a, b);
      break;
    default:
      throw std::invalid_argument("Unknown operator in process_operator");
      break;
  }
  return result;
}

double Calculator::process_function(type_t type, double a) {
  double result = NAN;
  switch (type) {
    case cosine:
      result = cos(a);
      break;
    case sine:
      result = sin(a);
      break;
    case tangent:
      result = tan(a);
      break;
    case a_cosine:
      result = acos(a);
      break;
    case a_sine:
      result = asin(a);
      break;
    case a_tangent:
      result = atan(a);
      break;
    case square_root:
      result = sqrt(a);
      break;
    case nlog:
      result = log(a);
      break;
    case logarithm:
      result = log10(a);
      break;
    default:
      throw std::invalid_argument("Unknown function in process_function");
      break;
  }
  return result;
}

}  // namespace rpn
