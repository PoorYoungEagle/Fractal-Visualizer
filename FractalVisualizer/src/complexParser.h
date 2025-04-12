#ifndef COMPLEX_PARSER_H
#define COMPLEX_PARSER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <cctype>

class ComplexExpressionParser {
public:
    std::string translate(const std::string& equation);

private:
    struct Token {
        enum Type { Number, Variable, Operator, Function, LeftParenthesis, RightParenthesis };
        Type type;
        std::string value;
        size_t pos;
    };

    std::vector<Token> tokens;
    size_t currentToken = 0;

    std::vector<Token> tokenize(const std::string& input);

    std::string parseExpression(int precedence = 0);
    std::string parsePrimary();

    static std::string capitalize(std::string s);
};

#endif // COMPLEX_PARSER_H