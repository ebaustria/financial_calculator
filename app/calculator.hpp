#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include "core/token.hpp"
#include <queue>
#include <stack>
#include <ui_calculator.h>
#include <utility>

class Calculator final : public QFrame
{
  Q_OBJECT

public:
  explicit Calculator(QWidget* parent = nullptr,
                      Qt::WindowFlags flags = Qt::WindowFlags());

private slots:
  // void update_equation();
  void calculate_result() const;

private:
  Operator multiplication{ "*" };
  Operator division{ "/" };
  Operator addition{ "+" };
  Operator subtraction{ "-" };
  Token left_par{ "(" };
  Token right_par{ ")" };
  Ui::CalculatorFrame calculator_frame;
  std::array<Token*, 6> operators;
  [[nodiscard]] std::vector<TokenPtr> tokenize() const;
  [[nodiscard]] bool is_operator(const std::string& str) const;
  static void process_operator(const TokenPtr& token,
                               std::stack<TokenPtr>& stack,
                               std::queue<TokenPtr>& out_queue);
  static void process_right_par(std::stack<TokenPtr>& stack,
                                std::queue<TokenPtr>& out_queue);
  [[nodiscard]] std::queue<TokenPtr> shunting_yard(
    const std::vector<TokenPtr>& tokens) const;
};

#endif /* CALCULATOR_HPP */
