#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

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
  void plot_simple_interest();
  void plot_loan_repayment();

private:
  Ui::CalculatorFrame calculator_frame;

  void connect_button(const QPushButton* button, char ch);
  void update_chart(const QString& title) const;
  ChartSeries chart_series;
  QChart* chart{ nullptr };
};

#endif /* CALCULATOR_HPP */
