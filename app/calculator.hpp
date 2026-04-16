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
  void from_currency_changed(int new_from_index);
  void to_currency_changed(int new_to_index);

private:
  Ui::CalculatorFrame calculator_frame;

  void connect_button(const QPushButton* button, char ch);
  static void update_axis_text(QAbstractAxis* axis, const QString& label);
  void update_chart(const QString& title, const QString& x_label) const;
  void set_up_chart() const;
  int from_currency_index;
  int to_currency_index;
  ChartSeries chart_series;
  QChart* chart{ nullptr };
};

#endif /* CALCULATOR_HPP */
