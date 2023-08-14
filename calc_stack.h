#ifndef CALC_STACK_H
#define CALC_STACK_H

#include <stack>
#include <string>

namespace rpn {

// Priority Scale
// 0: x number
// 1: + -
// 2: * / mod
// 3: ^ <Also () are in here>
// 4: functions

typedef enum {
  // Operators
  number,    // 0
  is_x,      // 1
  plus,      // 2
  minus,     // 3
  multiply,  // 4
  divide,    // 5
  power,     // 6
  modulus,   // 7
  // Functions
  cosine,       // 8
  sine,         // 9
  tangent,      // 10
  a_cosine,     // 11
  a_sine,       // 12
  a_tangent,    // 13
  square_root,  // 14
  nlog,         // 15
  logarithm,    // 16
  // Brackets
  open_bracket,  // 17
  close_bracket  // 18
} type_t;

class calc_stack {
 protected:
  class calc_node {
   public:
    double value = 0;
    int priority = 0;
    type_t type = number;
    calc_node(double v, int p, type_t t) : value(v), priority(p), type(t) {}
  };
  std::stack<calc_node> st;

 public:
  calc_stack() {}
  void load(std::string input);
  void move_element(std::stack<calc_node> &st1, std::stack<calc_node> &st2);

 private:
  void parse_string(std::string &data);
  double read_number(std::string::iterator &it);
  void simple_parse(std::string::iterator &it);
  void complex_parse(std::string::iterator &it);
  void flip_stack();
  void to_rpn();
};

}  // namespace rpn

#endif  // CALC_STACK_H
