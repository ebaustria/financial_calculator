#include "calculator.hpp"
#include "core/calc_helper_functions.hpp"

#include <QRegularExpressionValidator>
#include <iostream>
#include <regex>
#include <sstream>

// QRegularExpression Calculator::exp{ R"(^\D*\.\d*$)" };

Calculator::Calculator(QWidget* parent, const Qt::WindowFlags flags)
  : QFrame(parent, flags)
{
  calculator_frame.setupUi(this);
  calculator_frame.equationEdit->setValidator(new QRegularExpressionValidator(
    QRegularExpression("^[^A-Za-zÄÖÜäöüß]+$"), calculator_frame.equationEdit));

  operators[0] = &multiplication;
  operators[1] = &division;
  operators[2] = &addition;
  operators[3] = &subtraction;
  operators[4] = &left_par;
  operators[5] = &right_par;

  connect_button(calculator_frame.pushButton_0, '0');
  connect_button(calculator_frame.pushButton_1, '1');
  connect_button(calculator_frame.pushButton_2, '2');
  connect_button(calculator_frame.pushButton_3, '3');
  connect_button(calculator_frame.pushButton_4, '4');
  connect_button(calculator_frame.pushButton_5, '5');
  connect_button(calculator_frame.pushButton_6, '6');
  connect_button(calculator_frame.pushButton_7, '7');
  connect_button(calculator_frame.pushButton_8, '8');
  connect_button(calculator_frame.pushButton_9, '9');
  connect_button(calculator_frame.pushButton_lpar, '(');
  connect_button(calculator_frame.pushButton_rpar, ')');
  connect_button(calculator_frame.pushButton_div, '/');
  connect_button(calculator_frame.pushButton_add, '+');
  connect_button(calculator_frame.pushButton_min, '-');
  connect_button(calculator_frame.pushButton_mult, '*');
  connect_button(calculator_frame.pushButton_dec, '.');

  connect(calculator_frame.pushButton_del, &QPushButton::clicked, this, [this] {
    if (const QString text = calculator_frame.equationEdit->text();
        text.length() > 0) {
      calculator_frame.equationEdit->setText(text.chopped(1));
    }
  });

  // connect(calculator_frame.equationEdit, &QLineEdit::textChanged, this,
  // [this] {
  //   const QString text = calculator_frame.equationEdit->text();
  //   QRegularExpressionMatchIterator it = exp.globalMatch(text);
  //   QStringList matches;
  //
  //   while (it.hasNext()) {
  //     QRegularExpressionMatch match = it.next();
  //     QString cap = match.captured(0);
  //     cap.prepend('0');
  //     matches << cap;
  //   }
  //
  //   if (matches.count() > 0)
  //   {
  //     const QString edited_string = matches.join("");
  //     calculator_frame.equationEdit->setText(edited_string);
  //   }
  // });

  connect(calculator_frame.pushButton_eq,
          &QPushButton::clicked,
          this,
          &Calculator::calculate_result);
}

void
Calculator::connect_button(const QPushButton* button, char ch)
{
  connect(button, &QPushButton::clicked, this, [this, ch] {
    update_equation(QString(ch));
  });
}

void
Calculator::update_equation(const QString& str) const
{
  const QString text = calculator_frame.equationEdit->text();
  calculator_frame.equationEdit->setText(text + str);
}

void
Calculator::calculate_result() const
{
  try {
    std::queue<TokenPtr> out_queue = shunting_yard(tokenize());
    const std::string result = reverse_polish(out_queue);
    qDebug() << result;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

std::vector<TokenPtr>
Calculator::tokenize() const
{
  std::vector<TokenPtr> tokens;
  const std::string equation_text =
    calculator_frame.equationEdit->text().toStdString();
  const size_t equation_len = equation_text.length();
  std::string::size_type prev_pos = 0;

  for (std::string::size_type pos = 0; pos < equation_len; ++pos) {
    const std::string::size_type token_len = pos - prev_pos;
    if (std::string str{ equation_text[pos] }; is_operator(str)) {
      if (token_len > 0) {
        TokenPtr tp{ new Token{ equation_text.substr(prev_pos, token_len) } };
        tokens.push_back(tp);
      }
      if (str == "(" || str == ")") {
        TokenPtr tp{ new Token{ str } };
        tokens.push_back(tp);
        continue;
      }

      OperatorPtr op{ new Operator{ str } };
      tokens.push_back(op);
      prev_pos = pos + 1;
    }
    if (pos == equation_len - 1) {
      TokenPtr tp{ new Token{ equation_text.substr(prev_pos) } };
      tokens.push_back(tp);
    }
  }
  return tokens;
}

bool
Calculator::is_operator(const std::string& str) const
{
  bool is_operator{ false };
  for (const Token* op : operators) {
    if (str == op->value) {
      is_operator = true;
      break;
    }
  }
  return is_operator;
}

void
Calculator::process_operator(const TokenPtr& token,
                             std::stack<TokenPtr>& stack,
                             std::queue<TokenPtr>& out_queue)
{
  while (!stack.empty()) {
    if (stack.top()->value == "(") {
      break;
    }
    const OperatorPtr top = std::dynamic_pointer_cast<Operator>(stack.top());
    const OperatorPtr token_operator =
      std::dynamic_pointer_cast<Operator>(token);
    if (top->precedence > token_operator->precedence ||
        (top->precedence == token_operator->precedence &&
         token_operator->associativity == LEFT)) {
      out_queue.push(top);
      stack.pop();
    }
  }
  stack.push(token);
}

void
Calculator::process_right_par(std::stack<TokenPtr>& stack,
                              std::queue<TokenPtr>& out_queue)
{
  while (!stack.empty()) {
    if (stack.top()->value == "(") {
      break;
    }
    out_queue.push(stack.top());
    stack.pop();
  }
  if (stack.top()->value != "(") {
    throw std::runtime_error("Mismatched parentheses");
  }
  stack.pop();
}

std::queue<TokenPtr>
Calculator::shunting_yard(const std::vector<TokenPtr>& tokens) const
{
  std::stack<TokenPtr> stack;
  std::queue<TokenPtr> out_queue;

  for (const TokenPtr& tok : tokens) {
    if (tok->is_number()) {
      out_queue.push(tok);
      continue;
    }
    if (is_operator(tok->value) && tok->value != "(" && tok->value != ")") {
      process_operator(tok, stack, out_queue);
      continue;
    }
    if (tok->value == "(") {
      stack.push(tok);
      continue;
    }
    if (tok->value == ")") {
      process_right_par(stack, out_queue);
      continue;
    }
    std::stringstream string_stream;
    string_stream << "Token '" << tok->value
                  << "' is not a number or mathematical operator.";
    throw std::runtime_error(string_stream.str());
  }
  while (!stack.empty()) {
    if (stack.top()->value == "(") {
      throw std::runtime_error("Mismatched parentheses");
    }
    out_queue.push(stack.top());
    stack.pop();
  }
  return out_queue;
}
