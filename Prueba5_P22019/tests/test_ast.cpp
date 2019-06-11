#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <memory>
#include <unordered_map>
#include "doctest.h"
#include "expr_ast.h"

#define ADD(x, y) (new AddExpr((x), (y)))
#define SUB(x, y) (new SubExpr((x), (y)))
#define MUL(x, y) (new MulExpr((x), (y)))
#define DIV(x, y) (new DivExpr((x), (y)))
#define MOD(x, y) (new ModExpr((x), (y)))
#define NUM(n) (new NumExpr((n)))
#define ID(i) (new IdExpr((i)))

TEST_CASE("Simple expression") {
    ExprUPtr e = ADD(NUM(10), NUM(20));
    std::unordered_map<std::string, int> vars;
    
    CHECK(e->eval(vars) == 30);
}

TEST_CASE("Add expression") {
    ExprUPtr e = ADD(ID("s"), ADD(NUM(100), NUM(200)));
    std::unordered_map<std::string, int> vars;
    
    vars["s"] = 33;
    CHECK(e->eval(vars) == 333);
}

TEST_CASE("Sub expression") {
    ExprUPtr e = SUB(ID("s"), SUB(NUM(100), NUM(200)));
    std::unordered_map<std::string, int> vars;
    
    vars["s"] = 100;
    CHECK(e->eval(vars) == 200);
}

TEST_CASE("Mult expression") {
    ExprUPtr e = MUL(ID("s"), MUL(NUM(100), NUM(200)));
    std::unordered_map<std::string, int> vars;
    
    vars["s"] = 33;
    CHECK(e->eval(vars) == 660000);
}

TEST_CASE("Div expression") {
    ExprUPtr e = DIV(ID("s"), DIV(NUM(200), NUM(100)));
    std::unordered_map<std::string, int> vars;
    
    vars["s"] = 333;
    CHECK(e->eval(vars) == 166);
}

TEST_CASE("Mod expression") {
    ExprUPtr e = MOD(ID("s"), MOD(NUM(200), NUM(13)));
    std::unordered_map<std::string, int> vars;
    
    vars["s"] = 333;
    CHECK(e->eval(vars) == 3);
}

TEST_CASE("Combined expression (Add/Sub)") {
    ExprUPtr e = MUL( ADD(ID("x"), ID("y")),
                      SUB(NUM(567), NUM(560)) );
    std::unordered_map<std::string, int> vars;
    
    vars["x"] = 33;
    vars["y"] = 44;
    CHECK(e->eval(vars) == 539);
}

TEST_CASE("Combined expression (Div/Mod)") {
    ExprUPtr e = MOD( DIV( ADD(ID("x"), ID("y")), NUM(5) ), NUM(13) );
    std::unordered_map<std::string, int> vars;
    
    vars["x"] = 33;
    vars["y"] = 44;
    CHECK(e->eval(vars) == 2);
}
