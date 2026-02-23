
#pragma once

#include <unordered_map>
#include "tokens.h"

namespace lr_parser {

std::string token_to_string(Token t) {
    switch (t) {
        case Token::END:      return "$";
        case Token::ID:       return "id";
        case Token::ADD:      return "+";
        case Token::SUB:      return "-";
        case Token::MUL:      return "*";
        case Token::DIV:      return "/";
        case Token::LBRACKET: return "(";
        case Token::RBRACKET: return ")";
        case Token::ERROR:    return "ERROR";
        default:              return "UNKNOWN";
    }
}

enum struct NonTerminal { 
    E_BASE = 0, E, T, F
};

std::string nonterminal_to_string(NonTerminal t) {
    switch (t) {
        case NonTerminal::E_BASE: return "E'";
        case NonTerminal::E:      return "E";
        case NonTerminal::T:      return "T";
        case NonTerminal::F:      return "F";
        default:                  return "UNKNOWN";
    }
}

struct GrammarRule {
    NonTerminal nterm;
    unsigned len;
};

enum struct GrammarRuleType { 
    E_BASE = 0, // E' -> E
    E_ADD  = 1, // E -> E + T
    E_SUB  = 2, // E -> E - T
    E_T    = 3, // E -> T
    T_MUL  = 4, // T -> T * F
    T_DIV  = 5, // T -> T / F
    T_F    = 6, // T -> F
    F_EXPR = 7, // F -> ( E )
    F_ID   = 8  // F -> id
};

std::string grammar_to_string(GrammarRuleType t) {
    switch (t) {
        case GrammarRuleType::E_BASE: return "E' -> E";
        case GrammarRuleType::E_ADD:  return "E -> E + T";
        case GrammarRuleType::E_SUB:  return "E -> E - T";
        case GrammarRuleType::E_T:    return "E -> T";
        case GrammarRuleType::T_MUL:  return "T -> T * F";
        case GrammarRuleType::T_DIV:  return "T -> T / F";
        case GrammarRuleType::T_F:    return "T -> F";
        case GrammarRuleType::F_EXPR: return "F -> (E)";
        case GrammarRuleType::F_ID:   return "F -> id";
        default: return "UNKNOWN";
    }
}


class GrammarRules {
    std::unordered_map<GrammarRuleType, GrammarRule> grammar = {
        {GrammarRuleType::E_BASE, {NonTerminal::E_BASE, 1}},
        {GrammarRuleType::E_ADD,  {NonTerminal::E, 3}},
        {GrammarRuleType::E_SUB,  {NonTerminal::E, 3}},
        {GrammarRuleType::E_T,    {NonTerminal::E, 1}},
        {GrammarRuleType::T_MUL,  {NonTerminal::T, 3}},
        {GrammarRuleType::T_DIV,  {NonTerminal::T, 3}},
        {GrammarRuleType::T_F,    {NonTerminal::T, 1}},
        {GrammarRuleType::F_EXPR, {NonTerminal::F, 3}},
        {GrammarRuleType::F_ID,   {NonTerminal::F, 1}}
    };
public:
    const GrammarRule &get_rule(GrammarRuleType rtype) {
        return grammar[rtype];
    }
};
} // lr_parser