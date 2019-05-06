#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "lexer.h"
#include "parser.h"

std::vector<TokenInfo> error = {
    std::make_pair(Token::Ident, "x"),
    std::make_pair(Token::Eof, "EOF")
};

std::vector<TokenInfo> assign = {
    std::make_pair(Token::Ident, "x"),
    std::make_pair(Token::OpAssign, "="),
    std::make_pair(Token::OpenPar, "("),
    std::make_pair(Token::Number, "10"),
    std::make_pair(Token::OpAdd, "+"),
    std::make_pair(Token::Number, "7"),
    std::make_pair(Token::ClosePar, ")"),
    std::make_pair(Token::OpMul, "*"),
    std::make_pair(Token::Number, "5"),
    std::make_pair(Token::Eol, "\\n"),
    std::make_pair(Token::Eof, "EOF")
};

std::vector<TokenInfo> print = {
    std::make_pair(Token::KwPrint, "print"),
    std::make_pair(Token::OpenPar, "("),
    std::make_pair(Token::Number, "10"),
    std::make_pair(Token::OpAdd, "+"),
    std::make_pair(Token::Number, "7"),
    std::make_pair(Token::ClosePar, ")"),
    std::make_pair(Token::OpMul, "*"),
    std::make_pair(Token::Number, "5"),
    std::make_pair(Token::Eol, "\\n"),
    std::make_pair(Token::Eof, "EOF")
};

std::vector<TokenInfo> multiple = {
    std::make_pair(Token::Ident, "x"),
    std::make_pair(Token::OpAssign, "="),
    std::make_pair(Token::OpenPar, "("),
    std::make_pair(Token::Number, "10"),
    std::make_pair(Token::OpAdd, "+"),
    std::make_pair(Token::Number, "7"),
    std::make_pair(Token::ClosePar, ")"),
    std::make_pair(Token::OpMul, "*"),
    std::make_pair(Token::Number, "5"),
    std::make_pair(Token::Eol, "\\n"),
    std::make_pair(Token::KwPrint, "print"),
    std::make_pair(Token::Ident, "x"),
    std::make_pair(Token::Eol, "\\n"),
    std::make_pair(Token::Eof, "EOF")
};

bool throwOnError() {
    Lexer l(error);
    std::ostringstream out;
    Parser p(l, out); 

    bool throwException = false;
    try {
        p.parse();
    } catch (...) {
        throwException = true;
    }
    return throwException;
}

TEST_CASE("Assign statement") {
    REQUIRE(throwOnError);

    Lexer l(assign);
    std::ostringstream out;
    Parser p(l, out);

    bool parseSuccess = false;
    try {
        p.parse();
        parseSuccess = true;
    } catch (...) {
        parseSuccess = false;
    }
    REQUIRE(!l.hasTokens());
    CHECK(parseSuccess);
    CHECK(out.str() == "identifier=(number+number)*number;");
}

TEST_CASE("Print statement") {
    REQUIRE(throwOnError);

    Lexer l(print);
    std::ostringstream out;
    Parser p(l, out);

    bool parseSuccess = false;
    try {
        p.parse();
        parseSuccess = true;
    } catch (...) {
        parseSuccess = false;
    }
    REQUIRE(!l.hasTokens());
    CHECK(parseSuccess);
    CHECK(out.str() == "print((number+number)*number);");
}

TEST_CASE("Assign/Print") {
    REQUIRE(throwOnError);

    Lexer l(multiple);
    std::ostringstream out;
    Parser p(l, out);

    bool parseSuccess = false;
    try {
        p.parse();
        parseSuccess = true;
    } catch (...) {
        parseSuccess = false;
    }
    std::cout << l.getText();
    REQUIRE(!l.hasTokens());
    CHECK(parseSuccess);
    CHECK(out.str() == "identifier=(number+number)*number;print(identifier);");
}
