#include "calc_stack.h"

#include <cstring>
#include <iostream>

namespace rpn {

void calc_stack::parse_string(std::string &data) {
  for (std::string::iterator it = data.begin(); it != data.end(); ++it) {
    if (isdigit(*it)) {
      st.push(calc_node(read_number(it), 0, number));
    } else {
      simple_parse(it);
    }
  }
  flip_stack();
}

double calc_stack::read_number(std::string::iterator &it) {
  std::string::iterator begin = it;
  size_t num_len = 0;
  while (isdigit(*it) || *it == '.' || *it == 'e') {
    // Skip over sign after e
    if (*it == 'e' && (*(it + 1) == '-' || *(it + 1) == '+')) {
      ++it;
      ++num_len;
    }
    ++it;
    ++num_len;
  }
  // Since for loop is used in main function the iterator needs to be at the
  // last character of the number, not after it
  --it;
  std::string number_str(begin.base(), num_len);
  return std::stod(number_str);
}

void calc_stack::simple_parse(std::string::iterator &it) {
  switch (*it) {
    case '+':
      // Ignore unary plus
      if (!st.empty() && st.top().type != open_bracket) {
        st.push(calc_node(0, 1, plus));
      }
      break;
    case '-':
      // if unary - is present, push 0 before it (-1 -> 0-1)
      if (st.empty() || (st.top().type == open_bracket)) {
        st.push(calc_node(0, 0, number));
      }
      st.push(calc_node(0, 1, minus));
      break;
    case 'x':
      st.push(calc_node(0, 0, is_x));
      break;
    case '*':
      st.push(calc_node(0, 2, multiply));
      break;
    case '/':
      st.push(calc_node(0, 2, divide));
      break;
    case '^':
      st.push(calc_node(0, 3, power));
      break;
    case '(':
      st.push(calc_node(0, 3, open_bracket));
      break;
    case ')':
      st.push(calc_node(0, 3, close_bracket));
      break;
    default:
      complex_parse(it);
      break;
  }
}

void calc_stack::complex_parse(std::string::iterator &it) {
  if (strncmp(it.base(), "mod", 3) == 0) {
    st.push(calc_node(0, 2, modulus));
    it += 2;
  } else if (strncmp(it.base(), "cos", 3) == 0) {
    st.push(calc_node(0, 4, cosine));
    it += 2;
  } else if (strncmp(it.base(), "sin", 3) == 0) {
    st.push(calc_node(0, 4, sine));
    it += 2;
  } else if (strncmp(it.base(), "tan", 3) == 0) {
    st.push(calc_node(0, 4, tangent));
    it += 2;
  } else if (strncmp(it.base(), "acos", 4) == 0) {
    st.push(calc_node(0, 4, a_cosine));
    it += 3;
  } else if (strncmp(it.base(), "asin", 4) == 0) {
    st.push(calc_node(0, 4, a_sine));
    it += 3;
  } else if (strncmp(it.base(), "atan", 4) == 0) {
    st.push(calc_node(0, 4, a_tangent));
    it += 3;
  } else if (strncmp(it.base(), "sqrt", 4) == 0) {
    st.push(calc_node(0, 4, square_root));
    it += 3;
  } else if (strncmp(it.base(), "ln", 2) == 0) {
    st.push(calc_node(0, 4, nlog));
    it += 1;
  } else if (strncmp(it.base(), "log", 3) == 0) {
    st.push(calc_node(0, 4, logarithm));
    it += 2;
  } else {
    throw std::runtime_error("Unknown element in parse");
  }
}

void calc_stack::flip_stack() {
  std::stack<calc_node> flipped;
  while (!st.empty()) {
    move_element(flipped, st);
  }
  st = flipped;
}

void calc_stack::move_element(std::stack<calc_node> &st1,
                              std::stack<calc_node> &st2) {
  st1.push(st2.top());
  st2.pop();
}

// Shunting yard algorithm
void calc_stack::to_rpn() {
  std::stack<calc_node> rpn;
  std::stack<calc_node> secondary;
  while (!st.empty()) {
    // If node is a number
    if (st.top().priority == 0) {
      move_element(rpn, st);
    } else if (st.top().type != close_bracket) {
      if (!secondary.empty()) {
        if (secondary.top().priority < st.top().priority ||
            secondary.top().type == open_bracket ||
            st.top().type == open_bracket) {
        } else {
          // Push from secondary to rpn until <= priority is found
          while (!secondary.empty() &&
                 secondary.top().priority >= st.top().priority &&
                 secondary.top().type != open_bracket) {
            move_element(rpn, secondary);
          }
        }
      }
      move_element(secondary, st);
    } else {
      // Push from secondary to rpn until ( is found
      while (!secondary.empty() && secondary.top().type != open_bracket) {
        move_element(rpn, secondary);
      }
      if (secondary.empty()) {
        throw std::runtime_error("Opening bracket not found in to_rpn");
      }
      // Pop remaining (
      secondary.pop();
      st.pop();
    }
  }
  // Push the rest of secondary into rpn
  while (!secondary.empty()) {
    move_element(rpn, secondary);
  }
  st = rpn;
  flip_stack();
}

void calc_stack::load(std::string input) {
  parse_string(input);
  to_rpn();
}

}  // namespace rpn
