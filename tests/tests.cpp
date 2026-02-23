#include <gtest/gtest.h>
#include "parser.hpp"

class TestParser : public ::testing::Test {
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