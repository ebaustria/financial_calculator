#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <queue>
#include <stack>
#include <utility>
#include <ui_calculator.h>

enum Associativity
{
    LEFT,
    RIGHT,
};

struct Token
{
    explicit Token(std::string val) : value{std::move(val)}
    {
    }

    virtual ~Token() = default;

    [[nodiscard]] virtual bool is_number() const
    {
        char* p;
        strtof(value.c_str(), &p);
        return *p == 0;
    }

    std::string value;
};

struct Operator final : Token
{
    explicit Operator(const std::string& val) : Token(val)
    {
        if (val == "+" || val == "-")
        {
            precedence = 2;
        }
        else
        {
            precedence = 3;
        }
        associativity = LEFT;
    }

    [[nodiscard]] bool is_number() const override
    {
        return false;
    }

    uint8_t precedence{};
    Associativity associativity;
};

typedef std::shared_ptr<Token> TokenPtr;
typedef std::shared_ptr<Operator> OperatorPtr;

class Calculator final : public QFrame
{
    Q_OBJECT

public:
    explicit Calculator(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

private slots:
    // void update_equation();
    void calculate_result() const;

private:
    Operator multiplication{"*"};
    Operator division{"/"};
    Operator addition{"+"};
    Operator subtraction{"-"};
    Token left_par{"("};
    Token right_par{")"};
    Ui::CalculatorFrame calculator_frame;
    std::array<Token*, 6> operators;
    static float intermediate_result(const TokenPtr& operand_a, const TokenPtr& operand_b, const TokenPtr& op);
    static std::string reverse_polish(std::queue<TokenPtr>& out_queue);
    [[nodiscard]] std::vector<TokenPtr> tokenize() const;
    [[nodiscard]] bool is_operator(const std::string& str) const;
    static void process_operator(const TokenPtr& token, std::stack<TokenPtr>& stack, std::queue<TokenPtr>& out_queue);
    static void process_right_par(std::stack<TokenPtr>& stack, std::queue<TokenPtr>& out_queue);
    [[nodiscard]] std::queue<TokenPtr> shunting_yard(const std::vector<TokenPtr>& tokens) const;
};

#endif /* CALCULATOR_HPP */
