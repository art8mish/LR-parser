#include "parser.hpp"
#include <gtest/gtest.h>

class TestParser : public ::testing::Test {

public:
    using Action = lr_parser::Action;
    using State = lr_parser::State;
    using Grammar = lr_parser::GrammarRuleType;

protected:
    lr_parser::Parser parser;
};

TEST_F(TestParser, BasicIdentities) {
    parser.parse("a");
    EXPECT_TRUE(parser.is_accepted());

    parser.parse("(x)");
    EXPECT_TRUE(parser.is_accepted());
}

TEST_F(TestParser, AllOperators) {
    parser.parse("a + b");
    EXPECT_TRUE(parser.is_accepted());

    parser.parse("a - b");
    EXPECT_TRUE(parser.is_accepted());

    parser.parse("a * b");
    EXPECT_TRUE(parser.is_accepted());

    parser.parse("a / b");
    EXPECT_TRUE(parser.is_accepted());
}

TEST_F(TestParser, NestedParentheses) {
    parser.parse("(((a)))");
    EXPECT_TRUE(parser.is_accepted());

    parser.parse("(a + (b * (c / d)))");
    EXPECT_TRUE(parser.is_accepted());
}

TEST_F(TestParser, LongChain) {
    parser.parse("a + b - c * d / e + f");
    EXPECT_TRUE(parser.is_accepted());
}

TEST_F(TestParser, PrecedenceAndAssociativity) {
    parser.parse("a + b * c");
    EXPECT_TRUE(parser.is_accepted());

    parser.parse("a - b - c");
    EXPECT_TRUE(parser.is_accepted());
}

TEST_F(TestParser, WhitespaceTolerance) {
    parser.parse("  a + b  ");
    EXPECT_TRUE(parser.is_accepted());

    parser.parse("a\n+\tb");
    EXPECT_TRUE(parser.is_accepted());
}

TEST_F(TestParser, MissingOperands) {
    parser.parse("a + ");
    EXPECT_FALSE(parser.is_accepted());

    parser.parse("+ a");
    EXPECT_FALSE(parser.is_accepted());

    parser.parse("a * * b");
    EXPECT_FALSE(parser.is_accepted());
}

TEST_F(TestParser, BracketMismatches) {
    parser.parse("(a + b");
    EXPECT_FALSE(parser.is_accepted());

    parser.parse("a + b)");
    EXPECT_FALSE(parser.is_accepted());

    parser.parse("()");
    EXPECT_FALSE(parser.is_accepted());
}

TEST_F(TestParser, IllegalCharacters) {
    parser.parse("a @ b");
    EXPECT_FALSE(parser.is_accepted());
}

TEST_F(TestParser, CleanupAfterError) {
    parser.parse("a + + b");
    ASSERT_FALSE(parser.is_accepted());

    parser.parse("a + b");
    EXPECT_TRUE(parser.is_accepted());
}

TEST_F(TestParser, ControlSteps) {
    parser.parse("a * d - (a + (b - a)) / y");

    const auto &log = parser.get_action_log();

    std::vector<Action> expected = {
        // a * d
        Action::Shift(State::I5), Action::Reduce(Grammar::F_ID), Action::Reduce(Grammar::T_F),
        Action::Shift(State::I8), Action::Shift(State::I5), Action::Reduce(Grammar::F_ID),
        Action::Reduce(Grammar::T_MUL), Action::Reduce(Grammar::E_T),

        Action::Shift(State::I7),

        // (a + (b - a))
        Action::Shift(State::I4), Action::Shift(State::I5), Action::Reduce(Grammar::F_ID),
        Action::Reduce(Grammar::T_F), Action::Reduce(Grammar::E_T), Action::Shift(State::I6),

        // (b - a)
        Action::Shift(State::I4), Action::Shift(State::I5), Action::Reduce(Grammar::F_ID),
        Action::Reduce(Grammar::T_F), Action::Reduce(Grammar::E_T), Action::Shift(State::I7),
        Action::Shift(State::I5), Action::Reduce(Grammar::F_ID), Action::Reduce(Grammar::T_F),
        Action::Reduce(Grammar::E_SUB), Action::Shift(State::I15), Action::Reduce(Grammar::F_EXPR),

        Action::Reduce(Grammar::T_F), Action::Reduce(Grammar::E_ADD), Action::Shift(State::I15),
        Action::Reduce(Grammar::F_EXPR),

        // / y
        Action::Reduce(Grammar::T_F), Action::Shift(State::I9), Action::Shift(State::I5),
        Action::Reduce(Grammar::F_ID), Action::Reduce(Grammar::T_DIV),

        // -
        Action::Reduce(Grammar::E_SUB), Action::Accept()};

    ASSERT_EQ(log.size(), expected.size()) << "Wrong steps amount";

    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(log[i], expected[i]) << "Error on step " << i << "\n"
                                       << "Expected: "
                                       << (expected[i].is_shift()    ? "Shift"
                                           : expected[i].is_reduce() ? "Reduce"
                                                                     : "Accept");
    }
}