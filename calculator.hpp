#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <ui_calculator.h>

class Calculator : public QFrame
{
    Q_OBJECT
public:
    explicit Calculator(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

private:
    Ui::CalculatorFrame calculator_frame;
};

#endif /* CALCULATOR_HPP */