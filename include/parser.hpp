
#pragma once

#include <iomanip>
#include <optional>
#include <stack>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "ast.hpp"
#include "grammar.hpp"
#include "lexer.h"

namespace lr_parser {

enum struct ActionType { SHIFT, REDUCE, ACCEPT, ERROR };

enum struct State {
    I0 = 0,
    I1 = 1,   // GOTO(I0, E)
    I2 = 2,   // GOTO(I0, T)
    I3 = 3,   // GOTO(I0, F)
    I4 = 4,   // GOTO(I0, "(")
    I5 = 5,   // GOTO(I0, id)
    I6 = 6,   // GOTO(I1, +)
    I7 = 7,   // GOTO(I1, -)
    I8 = 8,   // GOTO(I2, *)
    I9 = 9,   // GOTO(I2, /)
    I10 = 10, // GOTO(I4, E)
    I11 = 11, // GOTO(I6, T)
    I12 = 12, // GOTO(I7, T)
    I13 = 13, // GOTO(I8, F)
    I14 = 14, // GOTO(I9, F)
    I15 = 15  // GOTO(I10, ")")
};

struct Action {
    //                                  |-> REDUCE
    std::variant<std::monostate, State, GrammarRuleType> value{};
    //           |-> ACCEPT      |-> SHIFT

    Action() = default;
    Action(State state) : value(state) {}
    Action(GrammarRuleType rule) : value(rule) {}

    static Action Accept() {
        return Action{};
    }
    static Action Shift(State state) {
        return Action{state};
    }
    static Action Reduce(GrammarRuleType rule) {
        return Action{rule};
    }

    bool is_accept() const {
        return std::holds_alternative<std::monostate>(value);
    }
    bool is_shift() const {
        return std::holds_alternative<State>(value);
    }
    bool is_reduce() const {
        return std::holds_alternative<GrammarRuleType>(value);
    }

    ActionType get_type() const {
        if (is_accept())
            return ActionType::ACCEPT;
        if (is_shift())
            return ActionType::SHIFT;
        return ActionType::REDUCE;
    }

    State get_state() const {
        return std::get<State>(value);
    }
    GrammarRuleType get_grammar_rule() const {
        return std::get<GrammarRuleType>(value);
    }

    bool operator==(const Action &rhs) const {
        return value == rhs.value;
    }
};

class Parser {
    GrammarRules grammar{};
    std::stack<State> state_stack{};
    std::vector<std::variant<Token, NonTerminal>> tokens{};

    std::stack<std::unique_ptr<Node>> node_stack{};
    std::unique_ptr<Node> ast{};
    // std::stack<std::string> symbol_stack;
    std::unordered_map<State, std::unordered_map<Token, Action>> action_table{};
    std::unordered_map<State, std::unordered_map<NonTerminal, State>> goto_table{};

    std::vector<Action> action_log{};

public:
    Parser() {
        init_action_table();
        init_goto_table();
    }

private:
    void init_action_table() {
        if (action_table.size() > 0)
            return;
        action_table[State::I0][Token::ID] = Action::Shift(State::I5);
        action_table[State::I0][Token::LBRACKET] = Action::Shift(State::I4);

        action_table[State::I1][Token::ADD] = Action::Shift(State::I6);
        action_table[State::I1][Token::SUB] = Action::Shift(State::I7);
        action_table[State::I1][Token::END] = Action::Accept();

        for (auto t : {Token::ADD, Token::SUB, Token::RBRACKET, Token::END})
            action_table[State::I2][t] = Action::Reduce(GrammarRuleType::E_T);
        action_table[State::I2][Token::MUL] = Action::Shift(State::I8);
        action_table[State::I2][Token::DIV] = Action::Shift(State::I9);

        for (auto t : {Token::ADD, Token::SUB, Token::MUL, Token::DIV, Token::RBRACKET, Token::END})
            action_table[State::I3][t] = Action::Reduce(GrammarRuleType::T_F);

        action_table[State::I4][Token::ID] = Action::Shift(State::I5);
        action_table[State::I4][Token::LBRACKET] = Action::Shift(State::I4);

        for (auto t : {Token::ADD, Token::SUB, Token::MUL, Token::DIV, Token::RBRACKET, Token::END})
            action_table[State::I5][t] = Action::Reduce(GrammarRuleType::F_ID);

        for (auto s : {State::I6, State::I7, State::I8, State::I9}) {
            action_table[s][Token::ID] = Action::Shift(State::I5);
            action_table[s][Token::LBRACKET] = Action::Shift(State::I4);
        }

        action_table[State::I10][Token::ADD] = Action::Shift(State::I6);
        action_table[State::I10][Token::SUB] = Action::Shift(State::I7);
        action_table[State::I10][Token::RBRACKET] = Action::Shift(State::I15);

        for (auto t : {Token::ADD, Token::SUB, Token::RBRACKET, Token::END})
            action_table[State::I11][t] = Action::Reduce(GrammarRuleType::E_ADD);
        action_table[State::I11][Token::MUL] = Action::Shift(State::I8);
        action_table[State::I11][Token::DIV] = Action::Shift(State::I9);

        for (auto t : {Token::ADD, Token::SUB, Token::RBRACKET, Token::END})
            action_table[State::I12][t] = Action::Reduce(GrammarRuleType::E_SUB);
        action_table[State::I12][Token::MUL] = Action::Shift(State::I8);
        action_table[State::I12][Token::DIV] = Action::Shift(State::I9);

        for (auto t : {Token::ADD, Token::SUB, Token::MUL, Token::DIV, Token::RBRACKET, Token::END})
            action_table[State::I13][t] = Action::Reduce(GrammarRuleType::T_MUL);

        for (auto t : {Token::ADD, Token::SUB, Token::MUL, Token::DIV, Token::RBRACKET, Token::END})
            action_table[State::I14][t] = Action::Reduce(GrammarRuleType::T_DIV);

        for (auto t : {Token::ADD, Token::SUB, Token::MUL, Token::DIV, Token::RBRACKET, Token::END})
            action_table[State::I15][t] = Action::Reduce(GrammarRuleType::F_EXPR);
    }

    void init_goto_table() {
        if (goto_table.size() > 0)
            return;
        goto_table[State::I0][NonTerminal::E] = State::I1;
        goto_table[State::I0][NonTerminal::T] = State::I2;
        goto_table[State::I0][NonTerminal::F] = State::I3;

        goto_table[State::I4][NonTerminal::E] = State::I10;
        goto_table[State::I4][NonTerminal::T] = State::I2;
        goto_table[State::I4][NonTerminal::F] = State::I3;

        goto_table[State::I6][NonTerminal::T] = State::I11;
        goto_table[State::I6][NonTerminal::F] = State::I3;

        goto_table[State::I7][NonTerminal::T] = State::I12;
        goto_table[State::I7][NonTerminal::F] = State::I3;

        goto_table[State::I8][NonTerminal::F] = State::I13;

        goto_table[State::I9][NonTerminal::F] = State::I14;
    }

    void print_header(const std::string &input) const {
        std::cout << "\nParsing expression: " << input << "\n";
        std::cout << std::string(108, '-') << "\n";
        std::cout << std::left << std::setw(7) << "| State" << " | " << std::setw(30) << "Stack"
                  << " | " << std::setw(30) << "Input" << " | " << std::setw(30) << "Action"
                  << " |\n";
        std::cout << std::string(108, '-') << "\n";
    }

    void print_end() const {
        std::cout << std::string(108, '-') << "\n";
    }

    void print_state(const std::string &input_str, const std::string &action_desc) const {
        std::string stack_str;
        for (const auto &t : tokens) {
            std::visit(
                [&stack_str, this](auto &&arg) {
                    using T = std::decay_t<decltype(arg)>;
                    if constexpr (std::is_same_v<T, Token>)
                        stack_str += token_to_string(arg) + " ";
                    else if constexpr (std::is_same_v<T, NonTerminal>)
                        stack_str += nonterminal_to_string(arg) + " ";
                },
                t);
        }

        std::cout << std::setw(7)
                  << "| I" + std::to_string(static_cast<unsigned>(state_stack.top())) << " | "
                  << std::setw(30) << stack_str << " | " << std::setw(30) << input_str << " | "
                  << std::setw(30) << action_desc << " | " << std::endl;
    }

    void cleanup() {
        if (!state_stack.empty()) {
            std::stack<State> empty_stack;
            state_stack.swap(empty_stack);
        }
        if (!node_stack.empty()) {
            std::stack<std::unique_ptr<Node>> empty_stack;
            node_stack.swap(empty_stack);
        }
        ast.reset();
        tokens.clear();
        action_log.clear();
    }

    void cut_input(std::string &input, const char *cut_text, size_t len) {
        size_t pos = input.find(cut_text);
        if (pos != std::string::npos)
            input.erase(pos, static_cast<size_t>(len));
        size_t non_space = input.find_first_not_of(" \t\n\r\f\v");
        if (non_space != std::string::npos)
            input.erase(0, non_space);
        else
            input.clear();
    }

    void ast_reduction(GrammarRuleType rule) {
        if (rule == GrammarRuleType::F_EXPR || rule == GrammarRuleType::E_T ||
            rule == GrammarRuleType::T_F || rule == GrammarRuleType::F_ID)
            return;

        BinOp op = (rule == GrammarRuleType::E_ADD)   ? BinOp::ADD
                   : (rule == GrammarRuleType::E_SUB) ? BinOp::SUB
                   : (rule == GrammarRuleType::T_MUL) ? BinOp::MUL
                                                      : BinOp::DIV;

        auto right = std::move(node_stack.top());
        node_stack.pop();
        auto left = std::move(node_stack.top());
        node_stack.pop();
        node_stack.push(std::make_unique<BinOpNode>(op, std::move(left), std::move(right)));
    }

public:
    void parse(const std::string &input) {
        YY_BUFFER_STATE buffer = yy_scan_string(input.c_str());

        cleanup();
        state_stack.push(State::I0);

        print_header(input);

        Token token = static_cast<Token>(yylex());
        std::string remaining_input = input + "$";

        while (true) {
            State state = state_stack.top();
            auto &action_row = action_table[state];

            if (action_row.find(token) == action_row.end()) {
                print_state(remaining_input, "Syntax error");
                print_end();
                std::cerr << "Syntax error: " << yytext << std::endl;
                break;
            }
            const Action &action = action_row[token];
            action_log.push_back(action);

            if (action.is_shift()) {
                State dst_state = action.get_state();
                print_state(remaining_input,
                            "Shift -> I" + std::to_string(static_cast<unsigned>(dst_state)));
                if (token == Token::ID)
                    node_stack.push(std::make_unique<IdNode>(yytext));
                state_stack.push(dst_state);
                tokens.push_back(token);

                cut_input(remaining_input, yytext, static_cast<size_t>(yyleng));

                token = static_cast<Token>(yylex());
            }

            else if (action.is_reduce()) {
                GrammarRuleType rule_type = action.get_grammar_rule();
                print_state(remaining_input, "Reduce " +
                                                 std::to_string(static_cast<unsigned>(rule_type)) +
                                                 ": " + grammar_to_string(rule_type));
                const GrammarRule &rule = grammar.get_rule(rule_type);

                ast_reduction(rule_type);
                for (unsigned i = 0; i < rule.len; ++i) {
                    state_stack.pop();
                    tokens.pop_back();
                }

                state = state_stack.top();
                state_stack.push(goto_table[state][rule.nterm]);
                tokens.push_back(rule.nterm);
            }

            else if (action.is_accept()) {
                print_state(remaining_input, "Accept");
                ast = std::move(node_stack.top());
                node_stack.pop();

                print_end();
                std::cout << "Success: Expression is valid\n";
                break;
            }

            else {
                print_state(remaining_input, "Unknown action");
                print_end();
                std::cerr << "Error: Unknown action" << std::endl;
                break;
            }
        }
        yy_delete_buffer(buffer);
    }

    const std::vector<Action> &get_action_log() const {
        return action_log;
    }

    bool is_accepted() const {
        return ast != nullptr;
    }

    void draw_ast(const std::string output_file = "output/ast.png") {
        if (!ast) {
            std::cerr << "Error: AST is not builded" << std::endl;
            return;
        }
        draw_ast_tree(ast.get(), output_file);
        std::cout << "AST saved: " << output_file << std::endl;
    }
};

} // namespace lr_parser