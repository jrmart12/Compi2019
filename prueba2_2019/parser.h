#ifndef _EXPR_PARSER_H
#define _EXPR_PARSER_H
#include "lexer.h"
class Parser {
public:
    Parser(Lexer &lexer,std::ostringstream &out): lexer(lexer),out(out){}
	void parse();
    void input();
    void opt_eol();
    void stmt_list();
    void stmt_list1();
    void stmt();
    void expr();
    void expr1();
    void term();
    void term1();
    void factor();

private:
    template<typename T>
    bool tokenIs(T tk) {
        return (currToken == tk);
    }

    template<typename T, typename... TArgs>
    bool tokenIs(T firstTk, TArgs... moreTk) {
        return (currToken == firstTk) || tokenIs(moreTk...);
    }
    Lexer& lexer;
    std::ostringstream &out;
    Token currToken;
    std::string text;
};

#endif
