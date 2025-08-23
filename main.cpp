#include "ui_calculator_window.h"
#include "ui_keypad.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    QFrame keypad;


    Ui::FinancialCalculatorWindow fin_calc_window;
    Ui::KeypadFrame keypad_frame;

    keypad_frame.setupUi(&keypad);
    fin_calc_window.setupUi(&window);

    window.setCentralWidget(&keypad);
    window.show();

    return app.exec();
}
