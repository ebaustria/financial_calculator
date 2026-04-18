#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <ui_calculator.h>

#include "core/chart_series.hpp"

constexpr int INVALID_CURRENCY_INDEX{ -1 };

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
  void from_currency_changed(int new_from_index);
  void to_currency_changed(int new_to_index);
  void currency_amount_changed(double new_amount);

private:
  Ui::CalculatorFrame calculator_frame;

  void connect_button(const QPushButton* button, char ch);
  static void update_axis_text(QAbstractAxis* axis, const QString& label);
  void update_chart(const QString& title, const QString& x_label) const;
  void update_conversion_result();
  void set_up_chart() const;
  double conversion_result{ 0.0 };
  int from_currency_index{ INVALID_CURRENCY_INDEX };
  int to_currency_index{ INVALID_CURRENCY_INDEX };
  ChartSeries chart_series;
  QChart* chart{ nullptr };
};

#endif /* CALCULATOR_HPP */
