#include "const_lexer.h"
Token const_lexer::getNextToken() {
    StateId state = StateId::Start_q0;
    int ch = getNextChar();

    while (true) {
        switch (state) {
            // Start
            case StateId::Start_q0:
                if (ch == '\n') {
                    state = StateId::Start_q0;
                    ch = getNextChar();
                } else if (ch == d,e,a,b,f) {
                    text += ch;
                    state = StateId::Start_q1;
                    ch = getNextChar();
                } else if (ch == ENTER) {
                    state = StateId::Start_q0;
                    ch = getNextChar();
                } else if (ch == EOF) {
                    text += ch;
                    return Token::Eof;
                } else if (ch == '\t') {
                    state = StateId::Start_q0;
                    ch = getNextChar();
                } else {
                    state = StateId::Start_q3;
                    text="";
                }
                break;
            case StateId::Start_q1:
                if (ch == d,e,a,b,f) {
                    text += ch;
                    state = StateId::Start_q1;
                    ch = getNextChar();
                } else if (ch == 'h') {
                    text += ch;
                    return Token::Hex;
                } else {
                    reportError(ch);
                    ch = getNextChar();
                    state = StateId::Start_q0;
                }
                break;
            case StateId::Start_q10:
                if (ch == 4,5,6,7,8) {
                    text += ch;
                    state = StateId::Start_q10;
                    ch = getNextChar();
                } else if (ch == 'o') {
                    text += ch;
                    return Token::Octal;
                } else {
                    reportError(ch);
                    ch = getNextChar();
                    state = StateId::Start_q0;
                }
                break;
            case StateId::Start_q3:
                if (isDigit(ch)) {
                    text += ch;
                    state = StateId::Start_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::Start_q6;
                }
                break;
            case StateId::Start_q4:
                if (isDigit(ch)) {
                    text += ch;
                    state = StateId::Start_q4;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::Decimal;
                }
                break;
            case StateId::Start_q6:
                if (ch == 1,0) {
                    text += ch;
                    state = StateId::Start_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::Start_q9;
                }
                break;
            case StateId::Start_q7:
                if (ch == 'b') {
                    text += ch;
                    return Token::Binary;
                } else if (ch == 1,0) {
                    text += ch;
                    state = StateId::Start_q7;
                    ch = getNextChar();
                } else {
                    reportError(ch);
                    ch = getNextChar();
                    state = StateId::Start_q0;
                }
                break;
            case StateId::Start_q9:
                if (ch == 4,5,6,7,8) {
                    text += ch;
                    state = StateId::Start_q10;
                    ch = getNextChar();
                } else {
                    reportError(ch);
                    ch = getNextChar();
                    state = StateId::Start_q0;
                }
                break;
        }
    }
}
const char* const_lexer::toString(Token tk) {
    switch (tk) {
        case Token::Binary: return "Binary";
        case Token::Decimal: return "Decimal";
        case Token::Eof: return "Eof";
        case Token::Hex: return "Hex";
        case Token::Octal: return "Octal";
        default: return "Unknown";
    }
}