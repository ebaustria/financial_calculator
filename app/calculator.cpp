#include "calculator.hpp"

#include <QRegularExpressionValidator>
#include <iostream>
#include <queue>
#include <regex>

#include "core/shunting_yard.hpp"
#include "core/strategy.hpp"

// QRegularExpression Calculator::exp{ R"(^\D*\.\d*$)" };

Calculator::Calculator(QWidget* parent, const Qt::WindowFlags flags)
  : QFrame(parent, flags)
{
  chart = new QChart;
  calculator_frame.setupUi(this);
  set_numeric_validator(calculator_frame.equationEdit);
  set_validators_for_numeric_form(calculator_frame.compoundForm);
  set_validators_for_numeric_form(calculator_frame.simpleInterestForm);
  set_validators_for_numeric_form(calculator_frame.loanRepaymentForm);

  const QDateTime now = QDateTime::currentDateTime();
  from = now.addMonths(-1);
  group = "week";
  calculator_frame.oneMonthRadioButton->toggle();

  const QStringList available_currencies{ "USD", "EUR", "GBP", "CHF", "AUD",
                                          "CAD", "INR", "JPY", "CNY" };
  calculator_frame.fromCurrencyComboBox->insertItems(0, available_currencies);
  calculator_frame.toCurrencyComboBox->insertItems(0, available_currencies);
  calculator_frame.fromCurrencyComboBox->setCurrentIndex(0);
  calculator_frame.toCurrencyComboBox->setCurrentIndex(1);

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
  connect(calculator_frame.oneMonthRadioButton,
          &QAbstractButton::toggled,
          this,
          &Calculator::one_month_radio_toggled);
  connect(calculator_frame.sixMonthsRadioButton,
          &QAbstractButton::toggled,
          this,
          &Calculator::six_month_radio_toggled);
  connect(calculator_frame.oneYearRadioButton,
          &QAbstractButton::toggled,
          this,
          &Calculator::one_year_radio_toggled);
  connect(calculator_frame.fiveYearsRadioButton,
          &QAbstractButton::toggled,
          this,
          &Calculator::five_year_radio_toggled);

  from_currency_index = calculator_frame.fromCurrencyComboBox->currentIndex();
  to_currency_index = calculator_frame.toCurrencyComboBox->currentIndex();
  calculator_frame.currencyConversionResult->setText(
    QString::number(conversion_result));
  calculator_frame.calculatorErrorLabel->setStyleSheet("color: red;");
  calculator_frame.currencyConversionErrorLabel->setStyleSheet("color: red;");
  calculator_frame.compoundInterestErrorLabel->setStyleSheet("color: red;");
  calculator_frame.simpleInterestErrorLabel->setStyleSheet("color: red;");
  calculator_frame.loanRepaymentErrorLabel->setStyleSheet("color: red;");

  set_up_chart();
  plot_conversion_rates();
}

Calculator::~Calculator()
{
  delete chart;
}

void
Calculator::set_validators_for_numeric_form(const QWidget* form)
{
  for (const auto child : form->children()) {
    if (auto* line_edit_ptr = dynamic_cast<QLineEdit*>(child);
        line_edit_ptr != nullptr) {
      set_numeric_validator(line_edit_ptr);
    }
  }
}

void
Calculator::set_numeric_validator(QLineEdit* line_edit)
{
  line_edit->setValidator(new QRegularExpressionValidator(
    QRegularExpression("^[^A-Za-zÄÖÜäöüß]+$"), line_edit));
}

void
Calculator::set_up_chart() const
{
  QSizePolicy sp_retain = calculator_frame.conversionRateTimespan->sizePolicy();
  sp_retain.setRetainSizeWhenHidden(true);
  calculator_frame.conversionRateTimespan->setSizePolicy(sp_retain);
  chart->legend()->hide();
  chart->addSeries(chart_context.line_series);
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
Calculator::one_month_radio_toggled(const bool checked)
{
  if (checked) {
    const QDateTime now = QDateTime::currentDateTime();
    from = now.addMonths(-1);
    group = "week";
    plot_conversion_rates();
  }
}

void
Calculator::six_month_radio_toggled(const bool checked)
{
  if (checked) {
    const QDateTime now = QDateTime::currentDateTime();
    from = now.addMonths(-6);
    group = "week";
    plot_conversion_rates();
  }
}

void
Calculator::one_year_radio_toggled(const bool checked)
{
  if (checked) {
    const QDateTime now = QDateTime::currentDateTime();
    from = now.addYears(-1);
    group = "week";
    plot_conversion_rates();
  }
}

void
Calculator::five_year_radio_toggled(const bool checked)
{
  if (checked) {
    const QDateTime now = QDateTime::currentDateTime();
    from = now.addYears(-5);
    group = "month";
    plot_conversion_rates();
  }
}

void
Calculator::plot_conversion_rates()
{
  try {
    ConversionStrategy currency_conversion_strat{
      calculator_frame.fromCurrencyComboBox->currentText(),
      calculator_frame.toCurrencyComboBox->currentText(),
      from,
      group
    };
    chart_context.set_strategy(&currency_conversion_strat);
    chart_context.replace_series();
    // TODO Handle title, axis labels here
    update_chart("Conversion Rate", "Something");
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

void
Calculator::currency_amount_changed(const double new_amount)
{
  if (from_currency_index != INVALID_CURRENCY_INDEX &&
      to_currency_index != INVALID_CURRENCY_INDEX) {
    update_conversion_result();
  }
  currency_amount = new_amount;
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
    plot_conversion_rates();
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
    plot_conversion_rates();
    update_conversion_result();
  }
}

void
Calculator::update_conversion_result()
{
  try {
    calculator_frame.conversionRateTimespan->setVisible(true);
    conversion_result = ConversionStrategy::convert_currency(
      calculator_frame.fromCurrencyComboBox->currentText(),
      calculator_frame.toCurrencyComboBox->currentText(),
      calculator_frame.fromCurrencySpinBox->value());
    calculator_frame.currencyConversionResult->setText(
      QString::number(conversion_result));
    calculator_frame.currencyConversionErrorLabel->clear();
  } catch (std::exception& e) {
    calculator_frame.currencyConversionErrorLabel->setText(e.what());
  }
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
    auto comp_int_strat = CompoundingInterestStrategy{
      calculator_frame.compoundPrincipalEdit->text(),
      calculator_frame.compoundLineEdit->text(),
      calculator_frame.compoundRateLineEdit->text(),
      calculator_frame.compoundYearsLineEdit->text()
    };
    chart_context.set_strategy(&comp_int_strat);
    chart_context.replace_series();
    update_chart("Compounding Interest", "Years");
    calculator_frame.compoundInterestErrorLabel->clear();
    calculator_frame.conversionRateTimespan->setVisible(false);
  } catch (std::exception& e) {
    calculator_frame.compoundInterestErrorLabel->setText(e.what());
  }
}

void
Calculator::plot_simple_interest()
{
  try {
    auto simple_int_strat =
      SimpleInterestStrategy{ calculator_frame.simplePrincipalLineEdit->text(),
                              calculator_frame.simpleLineEdit->text(),
                              calculator_frame.simpleYearsLineEdit->text() };
    chart_context.set_strategy(&simple_int_strat);
    chart_context.replace_series();
    update_chart("Simple Interest", "Years");
    calculator_frame.simpleInterestErrorLabel->clear();
    calculator_frame.conversionRateTimespan->setVisible(false);
  } catch (std::exception& e) {
    calculator_frame.simpleInterestErrorLabel->setText(e.what());
  }
}

void
Calculator::plot_loan_repayment()
{
  try {
    auto repayment_strat =
      LoanRepaymentStrategy{ calculator_frame.loanPrincipalEdit->text(),
                             calculator_frame.loanInterestEdit->text(),
                             calculator_frame.loanPaymentEdit->text() };
    chart_context.set_strategy(&repayment_strat);
    chart_context.replace_series();
    update_chart("Loan Repayment", "Months");
    calculator_frame.loanRepaymentErrorLabel->clear();
    calculator_frame.conversionRateTimespan->setVisible(false);
  } catch (std::exception& e) {
    calculator_frame.loanRepaymentErrorLabel->setText(e.what());
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

  x_axis->setRange(chart_context.strategy->min_x,
                   chart_context.strategy->max_x);
  update_axis_text(x_axis, x_label);

  y_axis->setRange(chart_context.strategy->min_y,
                   chart_context.strategy->max_y);
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
    calculator_frame.calculatorErrorLabel->clear();
  } catch (std::exception& e) {
    calculator_frame.calculatorErrorLabel->setText(e.what());
  }
}
