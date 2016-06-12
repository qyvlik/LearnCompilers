#ifndef LEXER_HPP
#define LEXER_HPP

#include "token.hpp"
#include "utility.hpp"
#include "throwable.hpp"

#include <string>
#include <vector>
#include <cctype>
#include <cassert>

class CodeStream
{
public:

    CodeStream(const std::string& s):
        code(s),
        index(0)
    { }

    char current() const {
        return code.at(index);
    }

    void next() throw(Throwable) {
        if(index < code.size()-1) {
            index++;
        } else {
            throw Throwable(0, "Can't Next Char");
        }
    }

    void rockBack() {
        if(index > 0) {
            index--;
        } else {
            throw Throwable(0, "Can't Rock Back Char");
        }
    }

    bool atEnd() const {
        return index == code.size() -1;
    }

private:
    std::string code;
    size_t index;
};

// if, else, while, for, do, break, continue, try, catch, throw, return, function
// bool, int, real, string, var

class StaticSymbol {
public:
    StaticSymbol():
        m_keyWord({ "if", "else", "while", "for", "do", "break", "continue", "try", "catch", "throw", "return", "function"}),
        m_typeName ( { "bool", "int", "real", "string", "var"}),
        m_operator( { '+', '-', '*', '/', '%', '=', '<', '>', '!' } )
    { }


    bool isKeyword(const std::string& token) const {
        return qyvlik::contains<std::string>(m_keyWord, token);
    }

    bool isTypeName(const std::string& token) const {
        return qyvlik::contains<std::string>(m_typeName, token);
    }

    bool isOperator(char v) const {
        return qyvlik::contains<char>(m_operator, v);
    }

    bool isHex(char v) const {
        static std::vector<char> hex{'0', '1', '2', '3', '4','5', '6', '7', '8', '9',
                                     'a', 'A', 'b', 'B', 'c', 'C', 'd', 'D', 'e', 'E', 'f', 'F'};
        return qyvlik::contains<char>(hex, v);
    }

    bool isWhiteSpace(char v) const {
        return std::isspace(v);
    }

    bool isIdentityPreFix(char v) const {
        return std::isalpha(v) || v == '_';
    }

    bool isNumber(char v) const {
        return std::isalnum(v);
    }

    bool isDelimiter(char v) const {
        static std::vector<char> delimiter { '[', ']', '(', ')', '{', '}', ';', '.', ',', ':' };
        return qyvlik::contains<char>(delimiter, v);
    }

private:
    std::vector<std::string> m_keyWord;
    std::vector<std::string> m_typeName;
    std::vector<char> m_operator;
};


class Lexer
{
public:

    static std::vector<Token> parser(CodeStream* stream, const StaticSymbol& symbol = StaticSymbol()) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        std::vector<Token> tokens;
        while(!stream->atEnd()) {
            if(symbol.isWhiteSpace(stream->current())) {
                WhiteSpace(stream, symbol);
            } else if(symbol.isIdentityPreFix(stream->current())) {
                tokens.push_back(Identity(stream, symbol));
            } else if(symbol.isNumber(stream->current())) {
                tokens.push_back(NumberLiteral(stream, symbol));
            } else if(stream->current() == '\'' || stream->current() == '\"') {
                try {
                    tokens.push_back(StringLiteral(stream, symbol));
                } catch(Throwable e) {
                    Throwable newE;
                    newE = e;
                    newE.setError("StringLiteral :" + e.getError());
                    throw newE;
                }
            } else if( symbol.isDelimiter(stream->current()) ) {
                tokens.push_back(Delimiter(stream, symbol));
            } else if( symbol.isOperator(stream->current()) ) {
                tokens.push_back(Operation(stream, symbol));
            } else {
                throw Throwable(0, std::string("Unknow Letter:") + stream->current() );
            }
        }
        return tokens;
    }

    // [, (, {
    static Token Delimiter(CodeStream* stream, const StaticSymbol& symbol) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        (void)symbol;
        std::string value;
        value.push_back(stream->current());
        stream->next();
        return Token(Token::Delimiter, value);
    }

    static Token Operation(CodeStream* stream, const StaticSymbol& symbol) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        std::string value;
        const bool is_NOT = stream->current() == '!';
        value.push_back(stream->current());

        stream->next();

        if(stream->current() != '=' && is_NOT) {
            throw Throwable(0, "Token `! Must Match Token `=");
        } else if( symbol.isOperator(stream->current()) ) {
            if(stream->current() == '-') {
                value.push_back(stream->current());
                stream->next();
                if(stream->current() == '>' || stream->current() == '=') {
                    value.push_back(stream->current());
                    stream->next();
                } else {
                    throw Throwable(0, "Token `- Must Match Token `- or `>");
                }
            } else {
                value.push_back(stream->current());
                stream->next();
            }
        }
        return Token(Token::Operator, value);
    }

    // 限制数字字面量
    // 0x
    // 0xFFFFFFFF
    // 0123456   [x]
    // 0.1
    // int:10 -> 10
    // int:16 -> 8
    // 0.11
    // 1.111
    // 10.1
    static Token NumberLiteral(CodeStream* stream, const StaticSymbol& symbol) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        const bool is_ZERO = stream->current() == '0';
        std::string value;
        std::string error;

        value.push_back(stream->current());
        stream->next();

        if(is_ZERO) {
            if(stream->current() == 'x') {
                value.push_back(stream->current());
                stream->next();
                int hex_number_length = 0;
                while(!stream->atEnd() && symbol.isHex(stream->current())) {
                    value.push_back(stream->current());
                    hex_number_length++;
                    stream->next();
                }
                if(hex_number_length > 8) {
                    throw Throwable(0, "Hex Number Is Too Long: " + std::to_string(hex_number_length));
                }
                return Token(Token::IntegerLiteral, value);
            } else if( symbol.isNumber(stream->current()) ){
                error.push_back(stream->current());
                throw Throwable(0, error + " Don't Support Number's Prefix Which is `0");
            } else if(stream->current() == '.') {
                // is double number
                value.push_back(stream->current());
                stream->next();
                // 小数长度
                int decimal_number_length = 0;
                while( !stream->atEnd() && symbol.isNumber(stream->current()) ) {
                    value.push_back(stream->current());
                    decimal_number_length++;

                    stream->next();
                }
                if(decimal_number_length > 15) {
                    throw Throwable(0, "Double Number's Decimal Part Is Too Long: " + std::to_string(decimal_number_length));
                }
                return Token(Token::DoubleLiteral, value);
            } else {
                error.push_back(stream->current());
                throw Throwable(0, "Unknow Error Current Letter : " + error);
            }
        } else {
            int dot_count = 0;
            int decimal_number_length = 0;
            int integer_number_length = 0;
            while( !stream->atEnd() && (symbol.isNumber(stream->current()) || stream->current() == '.') ) {
                if(stream->current() != '.') {
                    if(dot_count == 0) {
                        integer_number_length++;
                    } else {
                        decimal_number_length++;
                    }
                    value.push_back(stream->current());
                    stream->next();
                } else {
                    dot_count++;
                    if(dot_count > 2) {
                        throw Throwable(0, "Double Number Noly Has One Dot!");
                    }
                    value.push_back(stream->current());
                    stream->next();
                }
            }
            if(decimal_number_length > 15) {
                throw Throwable(0, "Double Number's Decimal Part Is Too Long: " + std::to_string(decimal_number_length));
            }
            if(integer_number_length > 10) {
                throw Throwable(0, "Integer Number's Is Too Long: " + std::to_string(decimal_number_length));
            }
            if(dot_count == 0) {
                return Token(Token::IntegerLiteral, value);
            } else {
                return Token(Token::DoubleLiteral, value);
            }
        }
    }

    // " '
    static Token StringLiteral(CodeStream* stream, const StaticSymbol& symbol) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        (void)symbol;
        std::string value;
        std::string error;
        char quota = stream->current();
        // TODO
        stream->next();         // eat `" `'

        // \\, \n, \', \"
        while(stream->current() != quota) {
            if(stream->current() == '\\') {
                stream->next();

                switch(stream->current())
                {
                case '\\':
                    value.push_back('\\');
                    break;
                case '\'':
                    value.push_back('\'');
                    break;
                case '\"':
                    value.push_back('\"');
                    break;
                case 'n':
                    value.push_back('\n');
                    break;
                default:
                    error.push_back(stream->current());
                    throw Throwable(0, error + " Not Escape Character !");
                }
            } else {

                value.push_back(stream->current());

            }
            stream->next();
        }

        if(stream->current() != quota) {
            throw Throwable(0, "StringLeterial Lost Last Quota");
        }
        stream->next();
        return Token(Token::StringLiteral, value);
    }

    // start of [a-zA-Z_]
    // include [a-zA-Z0-9_]
    static Token Identity(CodeStream* stream, const StaticSymbol& symbol) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        std::string value;
        value.push_back(stream->current());
        stream->next();

        while(std::isalnum(stream->current()) || std::isalpha(stream->current()) || stream->current() == '_') {
            value.push_back(stream->current());
            stream->next();
        }

        if(symbol.isTypeName(value)) {
            return Token(Token::TypeName, value);
        } else if(symbol.isKeyword(value)) {
            return Token(Token::KeyWord, value);
        } else {
            return Token(Token::Identity, value);
        }
    }

    static void WhiteSpace(CodeStream* stream, const StaticSymbol& symbol) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        while(symbol.isWhiteSpace(stream->current())) {
            stream->next();
        }
    }
};

#endif // LEXER_HPP

