#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include <gvc.h>
#include <graphviz/cgraph.h>

namespace lr_parser {

class Node {
protected:
    std::string id() const {
        return std::to_string(reinterpret_cast<size_t>(this));
    }
public:
    virtual ~Node() = default;
    virtual Agnode_t* draw(Agraph_t* g) const = 0;
};

class IdNode : public Node {
    std::string name;
public:
    IdNode(const std::string& name) : name(name) {}
    const std::string get_name() const {
        return name;
    }

    Agnode_t* draw(Agraph_t* g) const override {
        std::string node_id = "node_" + id();
        Agnode_t* node = agnode(g, const_cast<char*>(node_id.c_str()), 1);
        
        std::string label = "ID: " + name;
        agsafeset(node, const_cast<char*>("label"), const_cast<char*>(label.c_str()), const_cast<char*>(""));
        agsafeset(node, const_cast<char*>("shape"), const_cast<char*>("ellipse"), const_cast<char*>(""));
        return node;
    }
};

enum struct BinOp {
    ADD, SUB, MUL, DIV
};

std::string binop_to_string(BinOp op) {
    switch (op) {
        case BinOp::ADD: return "+";
        case BinOp::SUB: return "-";
        case BinOp::MUL: return "*";
        case BinOp::DIV: return "/";
        default: return "?";
    }
}

class BinOpNode : public Node {
    BinOp op_;
    std::unique_ptr<Node> left_;
    std::unique_ptr<Node> right_;

public:
    BinOpNode(BinOp op, std::unique_ptr<Node> left, std::unique_ptr<Node> right)
        : op_(op), left_(std::move(left)), right_(std::move(right)) {}

    Agnode_t* draw(Agraph_t* g) const override {
        std::string node_id = "node_" + id();
        Agnode_t* node = agnode(g, const_cast<char*>(node_id.c_str()), 1);

        std::string op_str = binop_to_string(op_);
        
        agsafeset(node, const_cast<char*>("label"), const_cast<char*>(op_str.c_str()), const_cast<char*>(""));
        agsafeset(node, const_cast<char*>("shape"), const_cast<char*>("circle"), const_cast<char*>(""));

        if (left_) {
            Agnode_t* l_child = left_->draw(g);
            agedge(g, node, l_child, nullptr, 1);
        }
        if (right_) {
            Agnode_t* r_child = right_->draw(g);
            agedge(g, node, r_child, nullptr, 1);
        }
        return node;
    }
};

void draw_ast_tree(std::unique_ptr<Node> root, const std::string& output_file) {
    if (!root) 
        return;

    GVC_t* gvc = gvContext();
    Agraph_t* g = agopen(const_cast<char*>("AST"), Agdirected, nullptr);

    agsafeset(g, const_cast<char*>("rankdir"), const_cast<char*>("TB"), const_cast<char*>(""));

    root->draw(g);

    gvLayout(gvc, g, "dot");
    gvRenderFilename(gvc, g, "png", output_file.c_str());

    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
}


} // lr_parser

