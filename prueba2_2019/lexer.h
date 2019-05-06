#ifndef _LEXER_H
#define _LEXER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

enum class Token { 
    KwPrint,
    Eol,
    OpenPar, 
    ClosePar, 
    OpAssign,
    OpAdd,
    OpSub,
    OpMul,
    OpDiv, 
    Number,
    Ident, 
    Unknown, 
    Eof
};

using TokenInfo = std::pair<Token, std::string>;

class Lexer {
public:
	Lexer(const std::vector<TokenInfo>& tklst): tklist(tklst) {
        it = tklist.begin();
    }

    int getLineNo() { return 1; }

    Token getNextToken() {
        Token tk = it->first;
        text = it->second;
        
        if (it != tklist.end()) {
            it++;
        }
        
        return tk;
    }
    
    bool hasTokens() { return it != tklist.end(); }
    std::string getText() { return text; }

private:
    std::string text;
    std::vector<TokenInfo> tklist;
    std::vector<TokenInfo>::iterator it;
};
#endif
