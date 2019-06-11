#ifndef _AST_H
#define _AST_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bits/unique_ptr.h>
#include <list>
#include <unordered_map>
#include <string>

class PExpr {
    public:
        PExpr(){};
		virtual int  eval(std::unordered_map<std::string, int> vars) = 0;

};

using ExprUPtr = PExpr *;

class BinaryExp : public PExpr {

    public:
        BinaryExp(ExprUPtr expr1, ExprUPtr expr2) : expr1(std::move(expr1)), expr2(std::move(expr2)) {}
		int eval(std::unordered_map<std::string, int> vars){};
        ExprUPtr expr1;
        ExprUPtr expr2;
};

#define Define_Binary_Expr(name) \
        class name##Expr : public BinaryExp {       \
            public: \
                name##Expr (ExprUPtr expr1,ExprUPtr expr2) : BinaryExp(std::move(expr1) , std::move(expr2)) {} \
					int eval(std::unordered_map<std::string, int> vars);\
        };  \

Define_Binary_Expr(Add);
Define_Binary_Expr(Sub);
Define_Binary_Expr(Mul);
Define_Binary_Expr(Div);
Define_Binary_Expr(Mod);

class IdExpr : public PExpr {
    public:
        IdExpr(std::string name) : name(name) {}
		int eval(std::unordered_map<std::string, int> vars){return 1;};
		std::string name;    
};
class NumExpr : public PExpr {
    public:
        NumExpr(int value) : value(value){};
        int eval(std::unordered_map<std::string, int> vars){return value;};
		 int value;
};

#endif
