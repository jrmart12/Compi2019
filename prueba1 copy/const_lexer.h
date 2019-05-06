#ifndef _EXPR_LEXER_H
#define _EXPR_LEXER_H

#include <fstream>
#include <iostream>
#include <string>
#include "lexer.h"
using std::ifstream;
using std::string;

class ConstLexer {

public:
    const_lexer(ifstream& in): in(in), line(1) {}
    Token getNextToken();
    string getText() { return text; }
    static const char* toString(Token tk);

private:
    ifstream& in;
    int line;
    string text;

    int getNextChar() { return in.get(); }

    void ungetChar(char c) { in.unget(); }
};
#endif