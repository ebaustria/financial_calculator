#include "calculator.hpp"
#include "core/calc_helper_functions.hpp"

#include <QRegularExpressionValidator>
#include <iostream>
#include <queue>
#include <regex>

#include "core/algo.hpp"
#include "core/currency_conversion.hpp"

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
  connect(calculator_frame.plotSimpleInterestButton,
          &QPushButton::clicked,
          this,
          &Calculator::plot_simple_interest);
  connect(calculator_frame.plotLoanRepaymentButton,
          &QPushButton::clicked,
          this,
          &Calculator::plot_loan_repayment);
  connect(calculator_frame.fromCurrencyComboBox,
          &QComboBox::currentIndexChanged,
          this,
          &Calculator::from_currency_changed);
  connect(calculator_frame.toCurrencyComboBox,
          &QComboBox::currentIndexChanged,
          this,
          &Calculator::to_currency_changed);
  connect(calculator_frame.fromCurrencySpinBox,
          &QDoubleSpinBox::valueChanged,
          this,
          &Calculator::currency_amount_changed);

  const QStringList available_currencies{ "USD", "EUR", "GBP", "CHF", "AUD",
                                          "CAD", "INR", "JPY", "CNY" };
  calculator_frame.fromCurrencyComboBox->insertItems(0, available_currencies);
  calculator_frame.toCurrencyComboBox->insertItems(0, available_currencies);
  calculator_frame.fromCurrencyComboBox->setCurrentIndex(0);
  calculator_frame.toCurrencyComboBox->setCurrentIndex(1);
  from_currency_index = calculator_frame.fromCurrencyComboBox->currentIndex();
  to_currency_index = calculator_frame.toCurrencyComboBox->currentIndex();
  calculator_frame.currencyConversionResult->setText(
    QString::number(conversion_result));

  set_up_chart();
}

Calculator::~Calculator()
{
  delete chart;
}

void
Calculator::set_up_chart() const
{
  chart->legend()->hide();
  chart->addSeries(chart_series.line_series);
  chart->setAnimationOptions(QChart::SeriesAnimations);
  chart->createDefaultAxes();
  chart->axes().at(0)->setLabelsBrush(QBrush(Qt::white));
  chart->axes().at(1)->setLabelsBrush(QBrush(Qt::white));
  chart->setTitle("No Data to Display");
  chart->setTitleBrush(QBrush(Qt::white));
  QFont font = chart->titleFont();
  font.setBold(true);
  font.setPointSize(12);
  chart->setTitleFont(font);
  chart->setBackgroundBrush(QBrush(QColor("#404040")));
  calculator_frame.lineChart->setRenderHint(QPainter::Antialiasing);
  calculator_frame.lineChart->setChart(chart);
}

void
Calculator::currency_amount_changed(double new_amount)
{
  if (from_currency_index != INVALID_CURRENCY_INDEX &&
      to_currency_index != INVALID_CURRENCY_INDEX) {
    update_conversion_result();
  }
}

void
Calculator::from_currency_changed(const int new_from_index)
{
  const bool swapped{ new_from_index ==
                      calculator_frame.toCurrencyComboBox->currentIndex() };
  if (swapped) {
    calculator_frame.toCurrencyComboBox->setCurrentIndex(from_currency_index);
  }

  from_currency_index = new_from_index;

  if (!swapped && from_currency_index != INVALID_CURRENCY_INDEX &&
      to_currency_index != INVALID_CURRENCY_INDEX) {
    update_conversion_result();
  }
}

void
Calculator::to_currency_changed(const int new_to_index)
{
  const bool swapped{ new_to_index ==
                      calculator_frame.fromCurrencyComboBox->currentIndex() };
  if (swapped) {
    calculator_frame.fromCurrencyComboBox->setCurrentIndex(to_currency_index);
  }

  to_currency_index = new_to_index;

  if (!swapped && from_currency_index != INVALID_CURRENCY_INDEX &&
      to_currency_index != INVALID_CURRENCY_INDEX) {
    update_conversion_result();
  }
}

void
Calculator::update_conversion_result()
{
  conversion_result =
    convert_currency(calculator_frame.fromCurrencyComboBox->currentText(),
                   calculator_frame.toCurrencyComboBox->currentText(),
                   calculator_frame.fromCurrencySpinBox->value());
  calculator_frame.currencyConversionResult->setText(
    QString::number(conversion_result));
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
    chart_series.replace_series();
    update_chart("Compounding Interest", "Years");
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

void
Calculator::plot_simple_interest()
{
  try {
    auto simple_int_strat = SimpleInterestStrategy{
      text_to_float(calculator_frame.simplePrincipalLineEdit),
      text_to_float(calculator_frame.simpleLineEdit),
      text_to_uint8(calculator_frame.simpleYearsLineEdit)
    };
    chart_series.set_strategy(&simple_int_strat);
    chart_series.replace_series();
    update_chart("Simple Interest", "Years");
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

void
Calculator::plot_loan_repayment()
{
  try {
    auto repayment_strat =
      LoanRepaymentStrategy{ text_to_float(calculator_frame.loanPrincipalEdit),
                             text_to_float(calculator_frame.loanInterestEdit),
                             text_to_float(calculator_frame.loanPaymentEdit) };
    chart_series.set_strategy(&repayment_strat);
    chart_series.replace_series();
    update_chart("Loan Repayment", "Months");
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

void
Calculator::update_axis_text(QAbstractAxis* axis, const QString& label)
{
  axis->setTitleText(label);
  axis->setTitleBrush(QBrush(Qt::white));
  QFont font = axis->titleFont();
  font.setBold(false);
  font.setPointSize(12);
  axis->setTitleFont(font);
}

void
Calculator::update_chart(const QString& title, const QString& x_label) const
{
  const auto x_axis = chart->axes(Qt::Horizontal).first();
  const auto y_axis = chart->axes(Qt::Vertical).first();

  x_axis->setRange(chart_series.strategy->min_x, chart_series.strategy->max_x);
  update_axis_text(x_axis, x_label);

  y_axis->setRange(chart_series.strategy->min_y, chart_series.strategy->max_y);
  update_axis_text(y_axis, "Currency");

  chart->setTitle(title);
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
