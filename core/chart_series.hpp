#ifndef FINANCIAL_CALCULATOR_CHART_SERIES_HPP
#define FINANCIAL_CALCULATOR_CHART_SERIES_HPP
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
  explicit Strategy(const QString& prin,
                    const QString& int_rate,
                    const QString& m_periods);
  virtual ~Strategy() = default;
  virtual double calculate(uint32_t period) = 0;
  virtual QList<QPointF> calculate_all();
  uint32_t max_periods;
  float principal;
  float interest_rate;
  uint8_t min_x{ 0 };
  uint32_t max_x{ 0 };
  double min_y{ 0 };
  double max_y{ 0 };

protected:
  InputChecker input_checker;
};

class CompoundingInterestStrategy : public Strategy
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

class SimpleInterestStrategy : public Strategy
{
public:
  SimpleInterestStrategy(const QString& prin,
                         const QString& int_rate,
                         const QString& m_periods)
    : Strategy{ prin, int_rate, m_periods }
  {
  }

  double calculate(uint32_t period) override;
};

class LoanRepaymentStrategy : public Strategy
{
public:
  LoanRepaymentStrategy(const QString& prin,
                        const QString& int_rate,
                        const QString& payment);

  QList<QPointF> calculate_all() override;
  double calculate(uint32_t period) override;
  [[nodiscard]] bool is_amortizing() const;

private:
  float monthly_payment;
  double monthly_interest;
};

class ChartSeries
{
public:
  ChartSeries();
  void set_strategy(Strategy* strat);
  void replace_series() const;

  QLineSeries* line_series{ nullptr };
  Strategy* strategy{ nullptr };
};

#endif // FINANCIAL_CALCULATOR_CHART_SERIES_HPP
