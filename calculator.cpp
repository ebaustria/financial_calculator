#include "calculator.hpp"

Calculator::Calculator(QWidget* parent, const Qt::WindowFlags flags)
    : QFrame(parent, flags)
{
    calculator_frame.setupUi(this);
}
