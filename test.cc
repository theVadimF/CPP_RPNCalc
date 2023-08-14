#include <gtest/gtest.h>

#include <cmath>

#include "calculator.h"
#include "validator.h"

#define EPS 1e-7

TEST(validate, test1) {
  bool res = rpn::validator::validate_string("1+1");
  EXPECT_TRUE(res);
}

TEST(validate, test2) {
  bool res = rpn::validator::validate_string("asdf");
  EXPECT_FALSE(res);
}

TEST(validate, test3) {
  bool res = rpn::validator::validate_string("-");
  EXPECT_FALSE(res);
}

TEST(validate, empty) {
  bool res = rpn::validator::validate_string("");
  EXPECT_FALSE(res);
}

TEST(validate, only_num) {
  bool res = rpn::validator::validate_string("3145.54");
  EXPECT_TRUE(res);
}

TEST(validate, incorrect_point) {
  bool res = rpn::validator::validate_string("3145.54.");
  EXPECT_FALSE(res);
}

TEST(validate, incorrect_point2) {
  bool res = rpn::validator::validate_string("3145.");
  EXPECT_FALSE(res);
}

TEST(validate, incorrect_point3) {
  bool res = rpn::validator::validate_string(".3145");
  EXPECT_FALSE(res);
}

TEST(validate, point) {
  bool res = rpn::validator::validate_string(".");
  EXPECT_FALSE(res);
}

TEST(validate, num_with_brackets) {
  bool res = rpn::validator::validate_string("(((3)))");
  EXPECT_TRUE(res);
}

TEST(validate, basic) {
  bool res = rpn::validator::validate_string("2.2*x-228/3.7");
  EXPECT_TRUE(res);
}

TEST(validate, function) {
  bool res = rpn::validator::validate_string("cos(x)");
  EXPECT_TRUE(res);
}

TEST(validate, mod) {
  bool res = rpn::validator::validate_string("6mod9");
  EXPECT_TRUE(res);
}

TEST(validate, complex) {
  bool res = rpn::validator::validate_string("sin(cos(2.6)/3*3.21+12)");
  EXPECT_TRUE(res);
}

TEST(validate, unary_function) {
  bool res = rpn::validator::validate_string("5.5*(-sin(x))");
  EXPECT_TRUE(res);
}

TEST(validate, point_x) {
  bool res = rpn::validator::validate_string("x.x");
  EXPECT_FALSE(res);
}

TEST(validate, test4) {
  bool res = rpn::validator::validate_string("((10^5)+ln(124.1))");
  EXPECT_TRUE(res);
}

TEST(validate, test5) {
  bool res =
      rpn::validator ::validate_string("(tan(121.41)/log(2342.1421)*acos(0))");
  EXPECT_TRUE(res);
}

TEST(validate, test6) {
  bool res = rpn::validator::validate_string("asin(0)+atan(0)*sqrt(64)");
  EXPECT_TRUE(res);
}

TEST(validate, test7) {
  bool res =
      rpn::validator ::validate_string("(tan(121.41)/log(420e-69+10)*acos(0))");
  EXPECT_TRUE(res);
}

TEST(validate, sci1) {
  bool res = rpn::validator::validate_string("10e-5");
  EXPECT_TRUE(res);
}

TEST(validate, sci2) {
  bool res = rpn::validator::validate_string("10e5");
  EXPECT_TRUE(res);
}

TEST(validate, sci3) {
  bool res = rpn::validator::validate_string("10e--5");
  EXPECT_FALSE(res);
}

TEST(validate, sci4) {
  bool res = rpn::validator::validate_string("10e+-5");
  EXPECT_FALSE(res);
}

TEST(validate, sci5) {
  bool res = rpn::validator::validate_string("10e-5e10");
  EXPECT_FALSE(res);
}

TEST(validate, sci6) {
  bool res = rpn::validator::validate_string("-10e-5e+10");
  EXPECT_FALSE(res);
}

TEST(validate, sci7) {
  bool res = rpn::validator::validate_string("-10e-5+10");
  EXPECT_TRUE(res);
}

TEST(validate, sci8) {
  bool res = rpn::validator::validate_string("10-(-10e-5)+10");
  EXPECT_TRUE(res);
}

TEST(calculate, test1) {
  rpn::Calculator calc("100-x/5");
  EXPECT_EQ(calc.calculate(15), 97);
}

TEST(calculate, test2) {
  rpn::Calculator calc("1+x+3+4*5*6^7");
  double expected = 5598727;
  double res = calc.calculate(3);
  EXPECT_LT(fabs(res - expected), EPS);
}

TEST(calculate, test3) {
  rpn::Calculator calc("cos(-006.1349*(-sin(x)))");
  double expected = -0.0107301;
  double res = calc.calculate(5.41);
  EXPECT_LT(fabs(res - expected), EPS);
}

TEST(calculate, brackets_basic) {
  rpn::Calculator calc("(420)");
  EXPECT_EQ(calc.calculate(0), 420);
}

TEST(calculate, brackets) {
  rpn::Calculator calc("(((((((((420)))))))))");
  EXPECT_EQ(calc.calculate(123), 420);
}

TEST(calculate, no_brackets) {
  rpn::Calculator calc("420");
  EXPECT_EQ(calc.calculate(123), 420);
}

TEST(calculate, pow) {
  rpn::Calculator calc("5^2");
  EXPECT_EQ(calc.calculate(123), 25);
}

TEST(calculate, ln) {
  rpn::Calculator calc("ln(100)");
  double expected = 4.6051701859880;
  double res = calc.calculate(5.41);
  EXPECT_LT(fabs(res - expected), EPS);
}

TEST(calculate, tan) {
  rpn::Calculator calc("tan(x)");
  double expected = 0;
  double res = calc.calculate(0);
  EXPECT_LT(fabs(res - expected), EPS);
}

TEST(calculate, log) {
  rpn::Calculator calc("log(100)");
  double expected = 2;
  double res = calc.calculate(0);
  EXPECT_LT(fabs(res - expected), EPS);
}

TEST(calculate, div) {
  rpn::Calculator calc("5/10");
  double expected = 0.5;
  double res = calc.calculate(0);
  EXPECT_LT(fabs(res - expected), EPS);
}

TEST(calculate, div_zero) {
  rpn::Calculator calc("x/0");
  double res = calc.calculate(10);
  EXPECT_NE(res, res);  // NAN check
}

TEST(calculate, mod) {
  rpn::Calculator calc("100mod7");
  double expected = 2;
  double res = calc.calculate(0);
  EXPECT_LT(fabs(res - expected), EPS);
}

TEST(calculate, mod_sci) {
  rpn::Calculator calc("10e1mod7");
  double expected = 2;
  double res = calc.calculate(0);
  EXPECT_LT(fabs(res - expected), EPS);
}

TEST(calculate, sci_negative) {
  rpn::Calculator calc("10e-1");
  double expected = 1;
  double res = calc.calculate(0);
  EXPECT_LT(fabs(res - expected), EPS);
}

TEST(calculate, mod_brackets) {
  rpn::Calculator calc("(((10e1)mod(7)))");
  double expected = 2;
  double res = calc.calculate(0);
  EXPECT_LT(fabs(res - expected), EPS);
}

TEST(calculate, sqrt) {
  rpn::Calculator calc("sqrt(100)");
  double expected = 10;
  double res = calc.calculate(0);
  EXPECT_LT(fabs(res - expected), EPS);
}

TEST(calculate, sqrt_sci) {
  rpn::Calculator calc("sqrt(10e1)");
  double expected = 10;
  double res = calc.calculate(0);
  EXPECT_LT(fabs(res - expected), EPS);
}

TEST(calculate, acos) {
  rpn::Calculator calc("acos(0)");
  double expected = M_PI / 2.0;
  double res = calc.calculate(0);
  EXPECT_LT(fabs(res - expected), EPS);
}

TEST(calculate, asin) {
  rpn::Calculator calc("asin(0)");
  double expected = 0;
  double res = calc.calculate(0);
  EXPECT_LT(fabs(res - expected), EPS);
}

TEST(calculate, atan) {
  rpn::Calculator calc("atan(0)");
  double expected = 0;
  double res = calc.calculate(0);
  EXPECT_LT(fabs(res - expected), EPS);
}

TEST(calculate, x) {
  rpn::Calculator calc("x+x");
  double expected = -4;
  double res = calc.calculate(-2);
  EXPECT_LT(fabs(res - expected), EPS);
}

TEST(calculate, just_x) {
  rpn::Calculator calc("x");
  double expected = -2;
  double res = calc.calculate(-2);
  EXPECT_LT(fabs(res - expected), EPS);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
