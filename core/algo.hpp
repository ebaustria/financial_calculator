#ifndef FINANCIAL_CALCULATOR_ALGO_HPP
#define FINANCIAL_CALCULATOR_ALGO_HPP

#include <memory>
#include <queue>
#include <sstream>
#include <stack>

#include "calc_helper_functions.hpp"
#include "token.hpp"

static std::vector<TokenPtr>
tokenize(const QString& text)
{
  std::vector<TokenPtr> tokens;
  const size_t equation_len = text.length();
  qsizetype prev_pos = 0;

  for (qsizetype pos = 0; pos < equation_len; ++pos) {
    const qsizetype token_len = pos - prev_pos;
    if (const QChar ch = text.at(pos); is_operator(ch) || is_separator(ch)) {
      if (token_len > 0) {
        TokenPtr tp{ new Token{ text.mid(prev_pos, token_len) } };
        tokens.push_back(tp);
      }
      if (is_separator(ch)) {
        TokenPtr tp{ new Token{ ch } };
        tokens.push_back(tp);
        prev_pos = pos + 1;
        continue;
      }

      OperatorPtr op{ new Operator{ ch } };
      tokens.push_back(op);
      prev_pos = pos + 1;
    }
    if (pos == equation_len - 1) {
      TokenPtr tp{ new Token{ text.mid(prev_pos) } };
      tokens.push_back(tp);
    }
  }
  return tokens;
}

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
    if (is_number(out_queue.front())) {
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

static void
process_operator(const TokenPtr& token,
                 std::stack<TokenPtr>& stack,
                 std::queue<TokenPtr>& out_queue)
{
  while (!stack.empty()) {
    if (stack.top()->value == "(") {
      break;
    }
    const OperatorPtr top = std::dynamic_pointer_cast<Operator>(stack.top());
    const OperatorPtr token_operator =
      std::dynamic_pointer_cast<Operator>(token);
    if (top->precedence > token_operator->precedence ||
        (top->precedence == token_operator->precedence &&
         token_operator->associativity == LEFT)) {
      out_queue.push(top);
      stack.pop();
    }
  }
  stack.push(token);
}

static void
process_right_par(std::stack<TokenPtr>& stack, std::queue<TokenPtr>& out_queue)
{
  while (!stack.empty()) {
    if (stack.top()->value == "(") {
      break;
    }
    out_queue.push(stack.top());
    stack.pop();
  }
  if (stack.top()->value != "(") {
    throw std::runtime_error("Mismatched parentheses");
  }
  stack.pop();
}

static std::queue<TokenPtr>
shunting_yard(const std::vector<TokenPtr>& tokens)
{
  std::stack<TokenPtr> stack;
  std::queue<TokenPtr> out_queue;

  for (const TokenPtr& tok : tokens) {
    if (is_number(tok)) {
      out_queue.push(tok);
      continue;
    }
    if (is_operator(tok->value) && !is_separator(tok->value)) {
      process_operator(tok, stack, out_queue);
      continue;
    }
    if (tok->value == "(") {
      stack.push(tok);
      continue;
    }
    if (tok->value == ")") {
      process_right_par(stack, out_queue);
      continue;
    }
    std::stringstream string_stream;
    string_stream << "Token '" << tok->value.toStdString()
                  << "' is not a number or mathematical operator.";
    throw std::runtime_error(string_stream.str());
  }
  while (!stack.empty()) {
    if (stack.top()->value == "(") {
      throw std::runtime_error("Mismatched parentheses");
    }
    out_queue.push(stack.top());
    stack.pop();
  }
  return out_queue;
}

#endif // FINANCIAL_CALCULATOR_ALGO_HPP
