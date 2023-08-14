#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>

namespace rpn {

class validator {
  typedef enum {
    nothing,
    number,
    oper,
    divider,
    divided_num,
    function,
    variable,
    bracket,
    sci_notation
  } element_t;

 public:
  static bool validate_string(std::string data);

 private:
  static bool is_unary(const std::string::iterator &it);
  static bool is_operator(std::string::iterator &it);
  static bool is_function(std::string::iterator &it);
  static void check_functions(std::string::iterator &iter, bool &pass,
                              int &brackets, element_t &prev_type);
  static bool check_number(std::string::iterator &iter);
};

}  // namespace rpn

#endif  // VALIDATOR_H
