#include "ui_calculator_window.h"
#include "ui_calculator.h"
#include "calculator.hpp"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QMainWindow window;
    Ui::FinancialCalculatorWindow fin_calc_window;
    Calculator calculator{&window};

    fin_calc_window.setupUi(&window);
    window.setCentralWidget(&calculator);
    window.show();

    return app.exec();
}
