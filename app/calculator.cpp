#include "calculator.hpp"
#include "core/calc_helper_functions.hpp"

#include <QRegularExpressionValidator>
#include <iostream>
#include <queue>
#include <regex>

#include "core/algo.hpp"

// QRegularExpression Calculator::exp{ R"(^\D*\.\d*$)" };

Calculator::Calculator(QWidget* parent, const Qt::WindowFlags flags)
  : QFrame(parent, flags)
{
  chart = new QChart;
  calculator_frame.setupUi(this);
  calculator_frame.equationEdit->setValidator(new QRegularExpressionValidator(
    QRegularExpression("^[^A-Za-zÄÖÜäöüß]+$"), calculator_frame.equationEdit));

  connect_button(calculator_frame.pushButton_0, '0');
  connect_button(calculator_frame.pushButton_1, '1');
  connect_button(calculator_frame.pushButton_2, '2');
  connect_button(calculator_frame.pushButton_3, '3');
  connect_button(calculator_frame.pushButton_4, '4');
  connect_button(calculator_frame.pushButton_5, '5');
  connect_button(calculator_frame.pushButton_6, '6');
  connect_button(calculator_frame.pushButton_7, '7');
  connect_button(calculator_frame.pushButton_8, '8');
  connect_button(calculator_frame.pushButton_9, '9');
  connect_button(calculator_frame.pushButton_lpar, '(');
  connect_button(calculator_frame.pushButton_rpar, ')');
  connect_button(calculator_frame.pushButton_div, '/');
  connect_button(calculator_frame.pushButton_add, '+');
  connect_button(calculator_frame.pushButton_min, '-');
  connect_button(calculator_frame.pushButton_mult, '*');
  connect_button(calculator_frame.pushButton_dec, '.');

  connect(calculator_frame.pushButton_del, &QPushButton::clicked, this, [this] {
    if (const QString text = calculator_frame.equationEdit->text();
        text.length() > 0) {
      calculator_frame.equationEdit->setText(text.chopped(1));
    }
  });

  connect(calculator_frame.pushButton_eq,
          &QPushButton::clicked,
          this,
          &Calculator::calculate_result);

  connect(calculator_frame.plotCompoundInterestButton,
          &QPushButton::clicked,
          this,
          &Calculator::plot_compounding_interest);
}

Calculator::~Calculator()
{
  delete chart;
}

void
Calculator::connect_button(const QPushButton* button, char ch)
{
  connect(button, &QPushButton::clicked, this, [this, ch] {
    update_equation(QString(ch));
  });
}

void
Calculator::update_equation(const QString& str) const
{
  const QString text = calculator_frame.equationEdit->text();
  calculator_frame.equationEdit->setText(text + str);
}

void
Calculator::plot_compounding_interest()
{
  try {
    auto comp_int_strat = CompoundingInterestStrategy(
      text_to_float(calculator_frame.compoundPrincipalEdit),
      text_to_float(calculator_frame.compoundLineEdit),
      text_to_float(calculator_frame.compoundRateLineEdit),
      text_to_uint8(calculator_frame.compoundYearsLineEdit));
    chart_series.set_strategy(&comp_int_strat);
    chart_series.fill_series();
    make_chart("Compounding Interest");
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

void
Calculator::make_chart(const QString& title) const
{
  chart->legend()->hide();
  chart->addSeries(chart_series.line_series);
  chart->createDefaultAxes();
  chart->setTitle(title);
  calculator_frame.lineChart->setChart(chart);
}

void
Calculator::calculate_result() const
{
  try {
    std::queue<TokenPtr> out_queue =
      shunting_yard(tokenize(calculator_frame.equationEdit->text()));
    const QString result = reverse_polish(out_queue);
    calculator_frame.resultEdit->setText(result);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}
