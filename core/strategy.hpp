#ifndef FINANCIAL_CALCULATOR_STRATEGY_HPP
#define FINANCIAL_CALCULATOR_STRATEGY_HPP
#include <QDateTime>
#include <QtCharts/QLineSeries>

class InputChecker
{
public:
  InputChecker() = default;
  virtual ~InputChecker() = default;
  virtual void check_input(const QString& input, const std::string& arg_name);
};

class InputCheckerDecorator : public InputChecker
{
public:
  explicit InputCheckerDecorator(InputChecker* input_checker);
  void check_input(const QString& input, const std::string& arg_name) override;

private:
  static bool is_integer(const QString& str);
  InputChecker* wrappee;
};

class Strategy
{
public:
  Strategy() = default;
  virtual ~Strategy() = default;
  virtual QList<QPointF> calculate_all() = 0;

  uint8_t min_x{ 0 };
  uint32_t max_x{ 0 };
  double min_y{ 0 };
  double max_y{ 0 };
};

class ConversionStrategy : public Strategy
{
public:
  explicit ConversionStrategy(QString base,
                              QString target,
                              const QDateTime& from,
                              std::string grouping);

  static double convert_currency(const QString& base,
                                 const QString& target,
                                 double amount);

  QList<QPointF> calculate_all() override;

private:
  QString base_currency;
  QString target_currency;
  QString from_date;
  std::string group;
};

class FinancialInstrumentStrat : public Strategy
{
public:
  explicit FinancialInstrumentStrat(const QString& prin,
                                    const QString& int_rate,
                                    const QString& m_periods);
  ~FinancialInstrumentStrat() override = default;
  virtual double calculate(uint32_t period) = 0;
  QList<QPointF> calculate_all() override;
  uint32_t max_periods;
  float principal;
  float interest_rate;

protected:
  InputChecker input_checker;
};

class CompoundingInterestStrategy : public FinancialInstrumentStrat
{
public:
  CompoundingInterestStrategy(const QString& prin,
                              const QString& int_rate,
                              const QString& comp_rate,
                              const QString& m_periods);

  double calculate(uint32_t period) override;

private:
  double compound_rate;
};

class SimpleInterestStrategy : public FinancialInstrumentStrat
{
public:
  SimpleInterestStrategy(const QString& prin,
                         const QString& int_rate,
                         const QString& m_periods)
    : FinancialInstrumentStrat{ prin, int_rate, m_periods }
  {
  }

  double calculate(uint32_t period) override;
};

class LoanRepaymentStrategy : public FinancialInstrumentStrat
{
public:
  LoanRepaymentStrategy(const QString& prin,
                        const QString& int_rate,
                        const QString& payment);

  QList<QPointF> calculate_all() final;
  double calculate(uint32_t period) override;
  [[nodiscard]] bool is_amortizing() const;

private:
  float monthly_payment;
  double monthly_interest;
};

#endif // FINANCIAL_CALCULATOR_STRATEGY_HPP
