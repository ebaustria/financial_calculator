#include "calculator.hpp"
#include "ui_calculator.h"
#include "ui_calculator_window.h"

int
main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  QMainWindow window;
  Ui::FinancialCalculatorWindow fin_calc_window;
  Calculator calculator{ &window };

  fin_calc_window.setupUi(&window);
  window.setCentralWidget(&calculator);
  window.show();

  return app.exec();
}
