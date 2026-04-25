#include "strategy.hpp"
#include "token.hpp"
#include <cmath>
#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <utility>

bool
InputCheckerDecorator::is_integer(const QString& str)
{
  const float fl = str.toFloat();
  return std::floor(fl) == fl;
}

InputCheckerDecorator::InputCheckerDecorator(InputChecker* input_checker)
  : wrappee{ input_checker }
{
}

void
InputCheckerDecorator::check_input(const QString& input,
                                   const std::string& arg_name)
{
  wrappee->check_input(input, arg_name);
  if (!is_integer(input)) {
    throw std::runtime_error(arg_name + " argument must be a whole number");
  }
}

void
InputChecker::check_input(const QString& input, const std::string& arg_name)
{
  if (!is_number(input)) {
    throw std::runtime_error(arg_name + " argument '" + input.toStdString() +
                             "' is not a number");
  }
  if (const float prin_float = input.toFloat(); prin_float < 0.0) {
    throw std::runtime_error(arg_name + " argument '" + input.toStdString() +
                             "' must be non-negative");
  }
}

ConversionStrategy::ConversionStrategy(QString base,
                                       QString target,
                                       const QDateTime& from,
                                       std::string grouping)
  : base_currency{ std::move(base) }
  , target_currency{ std::move(target) }
  , group{ std::move(grouping) }
{
  from_date = from.toString(Qt::ISODate).mid(0, 10);
}

QList<QPointF>
ConversionStrategy::calculate_all()
{
  QList<QPointF> new_points;
  std::stringstream url_sstream;

  url_sstream << "https://api.frankfurter.dev/v2/rates?from="
              << from_date.toStdString()
              << "&base=" << base_currency.toStdString()
              << "&quotes=" << target_currency.toStdString()
              << "&group=" << group;

  const cpr::Response r = cpr::Get(cpr::Url{ url_sstream.str() });

  if (r.error.code != cpr::ErrorCode::OK) {
    throw std::runtime_error(r.error.message);
  }

  auto json_response = nlohmann::json::parse(r.text);
  const uint32_t total_rates = json_response.size();
  max_x = total_rates - 1;
  min_y = std::numeric_limits<float>::max();
  max_y = std::numeric_limits<float>::min();

  for (size_t i = 0; i < total_rates; ++i) {
    auto obj = json_response[i];
    const double rate = json_response[i]["rate"];
    min_y = qMin(min_y, rate);
    max_y = qMax(max_y, rate);
    new_points.append(QPointF{ static_cast<double>(i), rate });
  }

  // TODO Move min and max handling to calculator class
  const double diff = max_y - min_y;
  const double buffer = diff * 0.1;
  max_y += buffer;
  min_y -= buffer;

  return new_points;
}

double
ConversionStrategy::convert_currency(const QString& base,
                                     const QString& target,
                                     const double amount)
{
  // const QDateTime now = QDateTime::currentDateTime();
  // const QDateTime yesterday = now.addDays(-1);
  // const QString now_str = now.toString(Qt::ISODate).mid(0, 10);
  // const QString yesterday_str = yesterday.toString(Qt::ISODate).mid(0, 10);
  std::stringstream url_sstream;

  if (amount == 0.0) {
    return amount;
  }

  url_sstream << "https://api.frankfurter.app/latest?amount="
              << std::to_string(amount) << "&from=" << base.toStdString()
              << "&to=" << target.toStdString();

  const cpr::Response r = cpr::Get(cpr::Url{ url_sstream.str() });

  if (r.error.code != cpr::ErrorCode::OK) {
    throw std::runtime_error(r.error.message);
  }

  auto json_response = nlohmann::json::parse(r.text);
  const double result = json_response["rates"][target.toStdString()];
  return result;
}

FinancialInstrumentStrat::FinancialInstrumentStrat(const QString& prin,
                                                   const QString& int_rate,
                                                   const QString& m_periods)
{
  input_checker.check_input(prin, "Principal");
  input_checker.check_input(int_rate, "Interest rate");

  InputCheckerDecorator input_checker_decorator{ &input_checker };

  input_checker_decorator.check_input(m_periods, "Years");

  principal = prin.toFloat();
  interest_rate = int_rate.toFloat();
  max_periods = m_periods.toUInt();
}

CompoundingInterestStrategy::CompoundingInterestStrategy(
  const QString& prin,
  const QString& int_rate,
  const QString& comp_rate,
  const QString& m_periods)
  : FinancialInstrumentStrat{ prin, int_rate, m_periods }
{
  InputCheckerDecorator input_checker_decorator{ &input_checker };
  input_checker_decorator.check_input(comp_rate, "Annual compound rate");

  compound_rate = comp_rate.toFloat();
}

LoanRepaymentStrategy::LoanRepaymentStrategy(const QString& prin,
                                             const QString& int_rate,
                                             const QString& payment)
  : FinancialInstrumentStrat{ prin, int_rate, QString{ "120" } }
{
  input_checker.check_input(payment, "Monthly payment");

  monthly_payment = payment.toFloat();
  const double int_rate_converted = interest_rate * 0.01;
  monthly_interest = int_rate_converted / 12;
}

QList<QPointF>
FinancialInstrumentStrat::calculate_all()
{
  QList<QPointF> new_points;
  max_x = max_periods;
  for (uint32_t period = 0; period <= max_periods; ++period) {
    const double final_value = calculate(period);
    new_points.append(QPointF{ static_cast<double>(period), final_value });
    if (period == 0) {
      min_y = final_value;
    }
    if (period == max_periods) {
      max_y = final_value;
    }
  }
  return new_points;
}

double
CompoundingInterestStrategy::calculate(const uint32_t period)
{
  const double int_rate_converted = interest_rate * 0.01;
  const double exponent = period * compound_rate;
  return principal * std::pow(1 + int_rate_converted / compound_rate, exponent);
}

double
SimpleInterestStrategy::calculate(const uint32_t period)
{
  const double int_rate_converted = interest_rate * 0.01;
  return principal * (1 + int_rate_converted * period);
}

QList<QPointF>
LoanRepaymentStrategy::calculate_all()
{
  QList<QPointF> new_points;
  if (is_amortizing()) {
    uint32_t period = 0;
    double remaining_balance = calculate(period);
    new_points.append(
      QPointF{ static_cast<double>(period), remaining_balance });
    max_y = remaining_balance;
    while (remaining_balance > 0.0) {
      period++;
      remaining_balance = calculate(period);
      new_points.append(
        QPointF{ static_cast<double>(period), remaining_balance });
    }
    max_x = period;
    min_y = remaining_balance;
  } else {
    new_points = FinancialInstrumentStrat::calculate_all();
  }
  return new_points;
}

double
LoanRepaymentStrategy::calculate(const uint32_t period)
{
  const double total_interest = std::pow(1 + monthly_interest, period);
  const double future_value = principal * total_interest;
  const double fv_annuity =
    monthly_payment * ((total_interest - 1) / monthly_interest);
  return std::max(0.0, future_value - fv_annuity);
}

bool
LoanRepaymentStrategy::is_amortizing() const
{
  return monthly_payment > principal * monthly_interest;
}
