#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <ui_calculator.h>

#include "core/chart_context.hpp"

constexpr int INVALID_CURRENCY_INDEX{ -1 };

class Calculator final : public QFrame
{
  Q_OBJECT

public:
  explicit Calculator(QWidget* parent = nullptr,
                      Qt::WindowFlags flags = Qt::WindowFlags());
  ~Calculator() override;

  Ui::CalculatorFrame calculator_frame;

private slots:
  void calculate_result() const;
  void update_equation(const QString& str) const;
  void plot_compounding_interest();
  void plot_simple_interest();
  void plot_loan_repayment();
  void from_currency_changed(int new_from_index);
  void to_currency_changed(int new_to_index);
  void currency_amount_changed(double new_amount);
  void one_month_radio_toggled(bool checked);
  void six_month_radio_toggled(bool checked);
  void one_year_radio_toggled(bool checked);
  void five_year_radio_toggled(bool checked);

private:
  void connect_button(const QPushButton* button, char ch);
  static void update_axis_text(QAbstractAxis* axis, const QString& label);
  static void set_validators_for_numeric_form(const QWidget* form);
  static void set_numeric_validator(QLineEdit* line_edit);
  void update_chart(const QString& title, const QString& x_label) const;
  void update_conversion_result();
  void set_up_chart() const;
  void plot_conversion_rates();
  double conversion_result{ 0.0 };
  double currency_amount{ 0.0 };
  QDateTime from;
  std::string group;
  int from_currency_index{ INVALID_CURRENCY_INDEX };
  int to_currency_index{ INVALID_CURRENCY_INDEX };
  ChartContext chart_context;
  QChart* chart{ nullptr };
};

#endif /* CALCULATOR_HPP */
