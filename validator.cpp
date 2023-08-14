#include "validator.h"

#include <vector>

#include "cstring"

namespace rpn {

bool validator::validate_string(std::string data) {
  bool pass = true;
  int brackets = 0;
  element_t prev_type = nothing;
  for (auto iter = data.begin(); pass && iter != data.end(); ++iter) {
    pass = false;
    if (*iter == 'x') {
      if (prev_type == nothing || prev_type == oper) {
        pass = true;
        prev_type = variable;
      }
    } else if (isdigit(*iter)) {
      pass = check_number(iter);
      prev_type = number;
    } else {
      check_functions(iter, pass, brackets, prev_type);
    }
  }

  if (pass && !(brackets == 0 && (prev_type == number || prev_type == bracket ||
                                  prev_type == variable))) {
    pass = false;
  }

  return pass;
}

void validator::check_functions(std::string::iterator &iter, bool &pass,
                                int &brackets, element_t &prev_type) {
  if (is_unary(iter)) {
    if (prev_type == nothing || prev_type == number || prev_type == variable ||
        prev_type == bracket) {
      pass = true;
      prev_type = oper;
    }
  } else if (is_operator(iter)) {
    if (prev_type == number || prev_type == variable || prev_type == bracket) {
      pass = true;
      prev_type = oper;
    }
  } else if (is_function(iter)) {
    if (prev_type == nothing || prev_type == oper || prev_type == bracket) {
      pass = true;
      prev_type = function;
    }
  } else if (*iter == '(') {
    if (prev_type == nothing || prev_type == oper || prev_type == function) {
      pass = true;
      prev_type = nothing;
      ++brackets;
    }
  } else if (*iter == ')') {
    if (brackets > 0 && (prev_type == number || prev_type == variable ||
                         prev_type == bracket)) {
      pass = true;
      prev_type = bracket;
      --brackets;
    }
  }
}

bool validator::is_operator(std::string::iterator &it) {
  bool pass = false;
  if (*it == '*' || *it == '/' || *it == '^') {
    pass = true;
  } else if (strncmp(it.base(), "mod", 3) == 0) {
    pass = true;
    it += 2;
  }
  return pass;
}

bool validator::is_unary(const std::string::iterator &it) {
  return (*it == '+' || *it == '-');
}

bool validator::is_function(std::string::iterator &it) {
  bool pass = false;
  std::vector<std::string> func_list{"ln",   "sin",  "cos",  "tan", "log",
                                     "acos", "asin", "atan", "sqrt"};

  for (auto f_it = func_list.begin(); !pass && f_it != func_list.end();
       ++f_it) {
    if (strncmp(it.base(), f_it->c_str(), f_it->length()) == 0) {
      pass = true;
      it += f_it->length() - 1;
    }
  }

  return pass;
}

bool validator::check_number(std::string::iterator &it) {
  bool pass = true;
  bool e_found = false;
  element_t type = number;
  while (pass && (isdigit(*it) || *it == '.' || *it == 'e')) {
    if (isdigit(*it)) {
      if (type == divider) {
        type = divided_num;
      } else if (type == sci_notation) {
        type = number;
      } else if (type != divided_num && type != number) {
        pass = false;
      }
    } else if (*it == '.') {
      if (type == number) {
        type = divider;
      } else {
        pass = false;
      }
    } else if (*it == 'e') {
      if (!e_found && (type == number || type == divided_num)) {
        type = sci_notation;
        e_found = true;
        if (*(it + 1) == '+' || *(it + 1) == '-') {
          ++it;
        }
      } else {
        pass = false;
      }
    }
    ++it;
  }

  if (pass && type != number && type != divided_num) {
    pass = false;
  }
  // Since for loop is used in main loop iterator needs to be at the end of the
  // number not after it
  --it;
  return pass;
}

}  // namespace rpn
