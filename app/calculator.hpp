#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include "core/token.hpp"
#include <queue>
#include <stack>
#include <ui_calculator.h>

#include "core/chart_series.hpp"

class Calculator final : public QFrame
{
  Q_OBJECT

public:
  explicit Calculator(QWidget* parent = nullptr,
                      Qt::WindowFlags flags = Qt::WindowFlags());
  ~Calculator() override;

private slots:
  void calculate_result() const;
  void update_equation(const QString& str) const;
  void plot_compounding_interest();

private:
  Operator multiplication{ "*" };
  Operator division{ "/" };
  Operator addition{ "+" };
  Operator subtraction{ "-" };
  Token left_par{ "(" };
  Token right_par{ ")" };
  Ui::CalculatorFrame calculator_frame;
  std::array<Token*, 6> operators;

  void connect_button(const QPushButton* button, char ch);
  void make_chart(const QString& title) const;
  [[nodiscard]] std::vector<TokenPtr> tokenize() const;
  [[nodiscard]] bool is_operator(const QString& str) const;
  static void process_operator(const TokenPtr& token,
                               std::stack<TokenPtr>& stack,
                               std::queue<TokenPtr>& out_queue);
  static void process_right_par(std::stack<TokenPtr>& stack,
                                std::queue<TokenPtr>& out_queue);
  [[nodiscard]] std::queue<TokenPtr> shunting_yard(
    const std::vector<TokenPtr>& tokens) const;
  ChartSeries chart_series;
  QChart* chart{ nullptr };
};

#endif /* CALCULATOR_HPP */
