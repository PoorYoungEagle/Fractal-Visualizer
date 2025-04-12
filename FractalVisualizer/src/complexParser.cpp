#include "complexParser.h"

std::string ComplexExpressionParser::translate(const std::string& equation) {
    tokens = tokenize(equation);
    currentToken = 0;
    return parseExpression();
}

std::vector<ComplexExpressionParser::Token> ComplexExpressionParser::tokenize(const std::string& input) {
    std::vector<Token> tokens;
    size_t pos = 0;

    while (pos < input.size()) {
        char c = input[pos];

        if (isspace(c)) {
            pos++;
            continue;
        }

        // Numbers
        if (isdigit(c) || c == '.') {
            size_t start = pos;
            while (pos < input.size() && (isdigit(input[pos]) || input[pos] == '.')) pos++;
            tokens.push_back({ Token::Number, input.substr(start, pos - start), start });
            continue;
        }

        // Variables and functions
        if (isalpha(c)) {
            size_t start = pos;
            while (pos < input.size() && isalnum(input[pos])) pos++;
            std::string ident = input.substr(start, pos - start);

            if (pos < input.size() && input[pos] == '(') {
                tokens.push_back({ Token::Function, ident, start });
                tokens.push_back({ Token::LeftParenthesis, "(", pos });
                pos++;
            }
            else {
                tokens.push_back({ Token::Variable, ident, start });
            }
            continue;
        }

        // Operators
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            tokens.push_back({ Token::Operator, std::string(1, c), pos });
            pos++;
            continue;
        }

        // Parentheses
        if (c == '(') {
            tokens.push_back({ Token::LeftParenthesis, "(", pos });
            pos++;
            continue;
        }
        if (c == ')') {
            tokens.push_back({ Token::RightParenthesis, ")", pos });
            pos++;
            continue;
        }

        throw std::runtime_error("Unexpected character at position " + std::to_string(pos) + ": " + std::string(1, c));
    }
    return tokens;
}

std::string ComplexExpressionParser::parseExpression(int precedence) {
    static const std::unordered_map<std::string, int> operatorPrecedence = {
        {"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}, {"^", 3}
    };

    std::string left = parsePrimary();

    while (currentToken < tokens.size()) {
        const Token& token = tokens[currentToken];
        if (token.type != Token::Operator) break;

        int currentPrecedence = operatorPrecedence.at(token.value);
        if (currentPrecedence <= precedence) break;

        currentToken++;
        std::string right = parseExpression(currentPrecedence);

        if (token.value == "^") {
            left = "complexPower(" + left + ", " + right + ")";
        }
        else if (token.value == "*") {
            left = "complexMultiply(" + left + ", " + right + ")";
        }
        else if (token.value == "/") {
            left = "complexDivide(" + left + ", " + right + ")";
        }
        else {
            left = "(" + left + token.value + right + ")";
        }
    }

    return left;
}

std::string ComplexExpressionParser::parsePrimary() {
    const Token& token = tokens[currentToken++];

    switch (token.type) {
        case Token::Number:
            return token.value;

        case Token::Variable:
            return token.value;

        case Token::Function: {
            std::string function = "complex" + capitalize(token.value);
            currentToken++; // skip (
            std::string arg = parseExpression();

            currentToken++; // skip )
            return function + "(" + arg + ")";
        }

        case Token::LeftParenthesis: {
            std::string expression = parseExpression();
            currentToken++; // skip )
            return "(" + expression + ")";
        }

        default:
            throw std::runtime_error("Unexpected token at position " + std::to_string(token.pos));
    }
}

std::string ComplexExpressionParser::capitalize(std::string s) {
    if (!s.empty()) {
        s[0] = toupper(s[0]);
    }
    return s;
}