%{
#include "tokens.h"
#include <iostream>
%}

%option header-file="include/lexer.h"
%option noyywrap

%%
[ \t\n\r]      { }
[a-zA-Z]+      { return static_cast<int>(Token::ID); }
[0-9]+         { return static_cast<int>(Token::NUM); }
"+"            { return static_cast<int>(Token::ADD); }
"-"            { return static_cast<int>(Token::SUB); }
"*"            { return static_cast<int>(Token::MUL); }
"/"            { return static_cast<int>(Token::DIV); }
"("            { return static_cast<int>(Token::LBRACKET); }
")"            { return static_cast<int>(Token::RBRACKET); }
.              { return static_cast<int>(Token::ERROR); }
%%