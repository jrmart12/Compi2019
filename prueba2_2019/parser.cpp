#include "parser.h"
#include "error.h"

void Parser::parse() {
    currToken=lexer.getNextToken();
    input();
}

void Parser::input(){
    stmt_list();
    opt_eol();
}

void Parser::opt_eol(){
    if(currToken==Token::Eol){
        currToken=lexer.getNextToken();
    }
    else{
                            out.str(text);
    }
}

void Parser::stmt_list(){
    stmt();
    stmt_list1();
}

void Parser::stmt_list1(){
    if(currToken==Token::Eol){
        currToken=lexer.getNextToken();
        stmt();
        stmt_list1();
    }
    else{
        stmt();
    }
}

void Parser::stmt(){
    if(currToken==Token::Ident){
        currToken=lexer.getNextToken();
        text+="identifier";
        if(currToken==Token::OpAssign){
            currToken=lexer.getNextToken();
            text+="=";
            expr();
                text+=";";
        }
    }
    else if(currToken==Token::KwPrint){
        currToken=lexer.getNextToken();
        text+="print(";
        expr();
        text+=")";
            text+=";";
    }
    else{
    }
}

void Parser::expr(){
    term();
    expr1();
}

void Parser::expr1(){
    if(currToken==Token::OpAdd){
        currToken=lexer.getNextToken();
        text+="+";
        term();
        expr1();
    }
    else if(currToken==Token::OpSub){
        currToken=lexer.getNextToken();
        text+="-";
        term();
        expr1();
    }
    else{
        term();
    }
}

void Parser::term(){
    factor();
    term1();
}

void Parser::term1(){
    if(currToken==Token::OpMul){
        currToken=lexer.getNextToken();
        text+="*";
        factor();
        term1();
    }
    else if(currToken==Token::OpDiv){
        currToken=lexer.getNextToken();
        text+="/";
        factor();
        term1();
    }
    else{
        factor();
    }
}

void Parser::factor(){
    if(currToken==Token::Ident){
        currToken=lexer.getNextToken();
        text+="identifier";
    }
    else if(currToken==Token::Number){
        currToken=lexer.getNextToken();
        text+="number";
    }
    else if(currToken==Token::OpenPar){
        currToken=lexer.getNextToken();
        text+="(";
        expr();
        if(currToken==Token::ClosePar){
            currToken=lexer.getNextToken();
            text+=")";
        }
        else{
        }
    }
}
