# CPP_RPNCalc
Calculator library for C++. This library is split into a calculator and a sepparate validator, make sure to validate user input using the validator before passing it into the calculator. Input should not have any spaces in it.

## Supported operations
- \+ \- \* \/ \^ mod
- cos
- sin
- tan
- acos
- asin
- atan
- sqrt
- ln
- log
- x (as a variable)

## Usage
### Validator
`bool validator::validate_string(std::string data)` returns true if the input is valid
### Calculator
```cpp
rpn::Calculator calc("1+(x+3+4)*5*6^7");
double result = calc.calculate(3);
```
