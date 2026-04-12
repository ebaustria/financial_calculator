#ifndef FINANCIAL_CALCULATOR_CHART_SERIES_HPP
#define FINANCIAL_CALCULATOR_CHART_SERIES_HPP
#include <QtCharts/QLineSeries>

class Strategy
{
public:
  explicit Strategy(const float prin,
                    const float int_rate,
                    const uint16_t m_periods)
    : principal{ prin }
    , interest_rate{ int_rate }
    , max_periods{ m_periods }
  {
  }
  virtual ~Strategy() = default;
  virtual double calculate(uint16_t period) = 0;
  virtual QList<QPointF> calculate_all();
  uint32_t max_periods;
  float principal;
  float interest_rate;
  uint8_t min_x{ 0 };
  uint32_t max_x{ 0 };
  double min_y{ 0 };
  double max_y{ 0 };
};

class CompoundingInterestStrategy : public Strategy
{
public:
  CompoundingInterestStrategy(const float prin,
                              const float int_rate,
                              const float comp_rate,
                              const uint16_t t_periods)
    : Strategy{ prin, int_rate, t_periods }
    , compound_rate{ comp_rate }
  {
  }

  double calculate(uint16_t period) override;

private:
  float compound_rate;
};

class SimpleInterestStrategy : public Strategy
{
public:
  SimpleInterestStrategy(const float prin,
                         const float int_rate,
                         const uint8_t t_periods)
    : Strategy{ prin, int_rate, t_periods }
  {
  }

  double calculate(uint16_t period) override;
};

class LoanRepaymentStrategy : public Strategy
{
public:
  LoanRepaymentStrategy(const float prin,
                        const float int_rate,
                        const float payment)
    : Strategy{ prin, int_rate, 120 }
    , monthly_payment{ payment }
  {
    const double int_rate_converted = interest_rate * 0.01;
    monthly_interest = int_rate_converted / 12;
  }

  QList<QPointF> calculate_all() override;
  double calculate(uint16_t period) override;
  bool is_amortizing() const;

private:
  float monthly_payment;
  double monthly_interest;
};

class ChartSeries
{
public:
  ChartSeries();
  ~ChartSeries();
  void set_strategy(Strategy* strat);
  void replace_series() const;

  QLineSeries* line_series{ nullptr };
  Strategy* strategy{ nullptr };
};

#endif // FINANCIAL_CALCULATOR_CHART_SERIES_HPP
