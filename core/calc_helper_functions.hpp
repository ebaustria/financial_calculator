#ifndef CALC_HELPER_FUNCTIONS_HPP
#define CALC_HELPER_FUNCTIONS_HPP

#include "core/token.hpp"
#include <queue>
#include <stack>

static double
intermediate_result(const TokenPtr& operand_a,
                    const TokenPtr& operand_b,
                    const TokenPtr& op)
{
  // TODO Fix this
  std::locale::global(std::locale::classic());
  const double a = operand_a->value.toDouble();
  const double b = operand_b->value.toDouble();
  if (op->value == "+") {
    return a + b;
  }
  if (op->value == "-") {
    return a - b;
  }
  if (op->value == "*") {
    return a * b;
  }
  return a / b;
}

static QString
reverse_polish(std::queue<TokenPtr>& out_queue)
{
  std::stack<TokenPtr> stack;
  while (!out_queue.empty()) {
    if (out_queue.front()->is_number()) {
      stack.push(out_queue.front());
      out_queue.pop();
    } else {
      const TokenPtr right = stack.top();
      stack.pop();
      const TokenPtr left = stack.top();
      stack.pop();
      // TODO Check precision here
      const TokenPtr result{ new Token{ QString::number(
        intermediate_result(left, right, out_queue.front()), 'g', 10) } };
      out_queue.pop();
      stack.push(result);
    }
  }
  return stack.top()->value;
}

#endif /* CALC_HELPER_FUNCTIONS_HPP */
