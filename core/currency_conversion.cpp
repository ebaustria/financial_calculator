#include "currency_conversion.hpp"

#include <QDateTime>
#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>

double
convert_currency(const QString& from, const QString& to, const double amount)
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
              << std::to_string(amount) << "&from=" << from.toStdString()
              << "&to=" << to.toStdString();

  const cpr::Response r = cpr::Get(cpr::Url{ url_sstream.str() });
  auto json_response = nlohmann::json::parse(r.text);
  const double result = json_response["rates"][to.toStdString()];
  return result;
}
